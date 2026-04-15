// ============================================================
// ClientNode.cc
// Implementation of the ClientNode module.
//
// Each client:
// 1. Reads the shared config (array of k elements, x subtasks)
// 2. Divides the array into x subtasks
// 3. Routes each subtask to client (subtaskId % N) via Chord
// 4. Processes incoming subtasks (finds local max)
// 5. Sends results back to the initiator via Chord routing
// 6. After receiving all results, computes consolidated max
// 7. Broadcasts a gossip message to all neighbors
// 8. Terminates after receiving gossip from all N clients
// ============================================================

#include "ClientNode.h"
#include "messages_m.h"

Define_Module(ClientNode);

// =====================================================================
//                         INITIALIZATION
// =====================================================================

void ClientNode::initialize()
{
    clientId = par("clientId");
    numClients = par("numClients");
    configFile = par("configFile").stdstringValue();

    taskCompleted = false;
    allGossipReceived = false;
    expectedResults = 0;

    // Open output file in append mode (shared across all clients)
    outputFile.open("outputfile.txt", std::ios::app);

    // Read task configuration (array, k, x)
    readConfig();

    // Build Chord finger table
    buildFingerTable();

    // Map neighbor IDs to output gate indices
    buildNeighborMap();

    // Determine ring successor and predecessor
    successor = (clientId + 1) % numClients;
    predecessor = (clientId - 1 + numClients) % numClients;

    // Log finger table
    std::string ftStr = "Client " + std::to_string(clientId) + " finger table: [";
    for (int i = 0; i < (int)fingerTable.size(); i++) {
        if (i > 0) ftStr += ", ";
        ftStr += std::to_string(fingerTable[i]);
    }
    ftStr += "]";
    EV << ftStr << endl;

    // Log neighbor map
    std::string nbStr = "Client " + std::to_string(clientId) + " neighbors: [";
    bool first = true;
    for (const auto &pair : neighborToGate) {
        if (!first) nbStr += ", ";
        nbStr += std::to_string(pair.first);
        first = false;
    }
    nbStr += "]";
    EV << nbStr << endl;

    // Schedule task initiation with staggered delay to avoid message storms
    // Each client starts 0.5s apart, beginning at t=1.0s
    cMessage *startMsg = new cMessage("start");
    scheduleAt(simTime() + clientId * 0.5 + 1.0, startMsg);

    EV << "Client " << clientId << " initialized. Task starts at t="
       << (clientId * 0.5 + 1.0) << "s" << endl;
}

// =====================================================================
//                        CONFIGURATION
// =====================================================================

void ClientNode::readConfig()
{
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw cRuntimeError("Client %d: Cannot open config file: %s",
                            clientId, configFile.c_str());
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        // Trim whitespace
        auto trim = [](std::string &s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end = s.find_last_not_of(" \t\r\n");
            s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
        };
        trim(key);
        trim(value);

        if (key == "k") {
            arraySize = std::stoi(value);
        } else if (key == "x") {
            numSubtasks = std::stoi(value);
        } else if (key == "array") {
            // Parse comma-separated array of integers
            std::stringstream ss(value);
            std::string item;
            while (std::getline(ss, item, ',')) {
                size_t s = item.find_first_not_of(" \t\r\n");
                size_t e = item.find_last_not_of(" \t\r\n");
                if (s != std::string::npos) {
                    item = item.substr(s, e - s + 1);
                    fullArray.push_back(std::stoi(item));
                }
            }
        }
    }
    file.close();

    // Validate array size
    if ((int)fullArray.size() != arraySize) {
        EV_WARN << "Client " << clientId << ": Array size mismatch (expected "
                << arraySize << ", got " << fullArray.size()
                << "). Using actual size." << endl;
        arraySize = fullArray.size();
    }

    // Ensure k/x >= 2 (each subtask has at least 2 elements)
    if (numSubtasks > arraySize / 2) {
        numSubtasks = arraySize / 2;
        EV_WARN << "Client " << clientId << ": Adjusted x to " << numSubtasks
                << " to satisfy k/x >= 2" << endl;
    }

    // Log constraint check: x > N
    if (numSubtasks <= numClients) {
        EV_WARN << "Client " << clientId << ": Warning - x (" << numSubtasks
                << ") should be > N (" << numClients << ")" << endl;
    }

    EV << "Client " << clientId << ": Config loaded - k=" << arraySize
       << ", x=" << numSubtasks << endl;
}

// =====================================================================
//                        CHORD ROUTING
// =====================================================================

void ClientNode::buildFingerTable()
{
    // Chord finger table: finger[i] = (clientId + 2^i) % N
    // for i = 0, 1, ..., ceil(log2(N)) - 1
    int m = (numClients > 1) ? (int)ceil(log2((double)numClients)) : 0;
    fingerTable.clear();
    for (int i = 0; i < m; i++) {
        int fingerTarget = (clientId + (1 << i)) % numClients;
        fingerTable.push_back(fingerTarget);
    }
}

void ClientNode::buildNeighborMap()
{
    // Map each connected neighbor's ID to the output gate index
    neighborToGate.clear();
    int numOut = gateSize("out");
    for (int i = 0; i < numOut; i++) {
        cGate *outGate = gate("out", i);
        if (outGate->isConnected()) {
            cGate *nextGate = outGate->getNextGate();
            if (nextGate) {
                cModule *nextModule = nextGate->getOwnerModule();
                int neighborId = nextModule->par("clientId");
                neighborToGate[neighborId] = i;
            }
        }
    }
}

bool ClientNode::isInOpenInterval(int x, int a, int b)
{
    // Returns true if x is in the open interval (a, b) on
    // a circular ring of size numClients: [0, numClients)
    if (a == b) return false;
    if (a < b) {
        return (x > a && x < b);
    } else {
        // Wraps around 0
        return (x > a || x < b);
    }
}

bool ClientNode::isInHalfOpenInterval(int x, int a, int b)
{
    // Returns true if x is in the half-open interval (a, b]
    // on a circular ring
    if (a == b) return false;
    if (x == b) return true;
    return isInOpenInterval(x, a, b);
}

int ClientNode::findNextHop(int targetId)
{
    // --- Optimization: direct connection to target ---
    if (neighborToGate.find(targetId) != neighborToGate.end()) {
        return targetId;
    }

    // --- Standard Chord lookup ---
    // If target is between us and our successor, forward to successor
    if (isInHalfOpenInterval(targetId, clientId, successor)) {
        return successor;
    }

    // Find the closest preceding finger node that we're connected to
    for (int i = (int)fingerTable.size() - 1; i >= 0; i--) {
        if (isInOpenInterval(fingerTable[i], clientId, targetId)) {
            if (neighborToGate.find(fingerTable[i]) != neighborToGate.end()) {
                return fingerTable[i];
            }
        }
    }

    // Fallback: find the best neighbor preceding the target
    int bestHop = successor;
    for (const auto &pair : neighborToGate) {
        int neighborId = pair.first;
        if (isInOpenInterval(neighborId, clientId, targetId)) {
            // Pick the neighbor closest to target (furthest from us toward target)
            if (bestHop == successor ||
                isInOpenInterval(bestHop, clientId, neighborId)) {
                bestHop = neighborId;
            }
        }
    }

    return bestHop;
}

void ClientNode::routeMessage(cMessage *msg, int targetId)
{
    // Local delivery: schedule as self-message with tiny delay
    if (targetId == clientId) {
        scheduleAt(simTime() + 0.001, msg);
        return;
    }

    int nextHop = findNextHop(targetId);

    // Safety check: ensure we have a gate to the next hop
    if (neighborToGate.find(nextHop) == neighborToGate.end()) {
        EV_ERROR << "Client " << clientId
                 << ": ROUTING ERROR - no gate to next hop " << nextHop
                 << " for target " << targetId << endl;
        delete msg;
        return;
    }

    int gateIdx = neighborToGate[nextHop];

    // Update routing metadata in the message
    if (SubtaskMsg *stMsg = dynamic_cast<SubtaskMsg *>(msg)) {
        stMsg->setSourceClientId(clientId);
        stMsg->setHopCount(stMsg->getHopCount() + 1);
    } else if (SubtaskResultMsg *srMsg = dynamic_cast<SubtaskResultMsg *>(msg)) {
        srMsg->setSourceClientId(clientId);
        srMsg->setHopCount(srMsg->getHopCount() + 1);
    }

    EV << "Client " << clientId << ": Routing to " << targetId
       << " via hop " << nextHop << " (gate " << gateIdx << ")" << endl;

    send(msg, "out", gateIdx);
}

// =====================================================================
//                       TASK DISTRIBUTION
// =====================================================================

void ClientNode::divideAndDistribute()
{
    logOutput("============================================================");
    logOutput("Client " + std::to_string(clientId) + ": TASK INITIATION");
    logOutput("============================================================");

    // Display the full array
    std::string arrayStr = "Array (" + std::to_string(arraySize) + " elements): [";
    for (int i = 0; i < arraySize; i++) {
        if (i > 0) arrayStr += ", ";
        arrayStr += std::to_string(fullArray[i]);
    }
    arrayStr += "]";
    logOutput(arrayStr);
    logOutput("Subtasks (x): " + std::to_string(numSubtasks) +
              " | Clients (N): " + std::to_string(numClients));
    logOutput("");

    // Divide the array into x approximately equal parts
    int chunkSize = arraySize / numSubtasks;
    int remainder = arraySize % numSubtasks;
    expectedResults = numSubtasks;

    int offset = 0;
    for (int i = 0; i < numSubtasks; i++) {
        // First 'remainder' subtasks get one extra element
        int thisChunkSize = chunkSize + (i < remainder ? 1 : 0);
        int targetClient = i % numClients;

        // Create subtask message
        SubtaskMsg *msg = new SubtaskMsg("subtask");
        msg->setSubtaskId(i);
        msg->setInitiatorId(clientId);
        msg->setTargetClientId(targetClient);
        msg->setSourceClientId(clientId);
        msg->setHopCount(0);
        msg->setElementsArraySize(thisChunkSize);

        // Fill in the sub-array elements
        std::string elemStr = "";
        for (int j = 0; j < thisChunkSize; j++) {
            msg->setElements(j, fullArray[offset + j]);
            if (j > 0) elemStr += ", ";
            elemStr += std::to_string(fullArray[offset + j]);
        }

        std::string routeType = (targetClient == clientId) ? " [LOCAL]" : " [REMOTE]";
        logOutput("  Subtask " + std::to_string(i) +
                  " -> Client " + std::to_string(targetClient) +
                  ": [" + elemStr + "]" + routeType);

        offset += thisChunkSize;

        // Route subtask to the target client via Chord
        routeMessage(msg, targetClient);
    }
    logOutput("");
}

// =====================================================================
//                       MESSAGE HANDLING
// =====================================================================

void ClientNode::handleMessage(cMessage *msg)
{
    // Handle the "start" self-message that triggers task initiation
    if (msg->isSelfMessage() && strcmp(msg->getName(), "start") == 0) {
        divideAndDistribute();
        delete msg;
        return;
    }

    // Route or process based on message type
    if (SubtaskMsg *stMsg = dynamic_cast<SubtaskMsg *>(msg)) {
        if (stMsg->getTargetClientId() == clientId) {
            handleSubtask(stMsg);
        } else {
            // Not for us - forward via Chord routing
            routeMessage(stMsg, stMsg->getTargetClientId());
        }
    }
    else if (SubtaskResultMsg *srMsg = dynamic_cast<SubtaskResultMsg *>(msg)) {
        if (srMsg->getTargetClientId() == clientId) {
            handleSubtaskResult(srMsg);
        } else {
            // Not for us - forward via Chord routing
            routeMessage(srMsg, srMsg->getTargetClientId());
        }
    }
    else if (GossipMsg *gMsg = dynamic_cast<GossipMsg *>(msg)) {
        handleGossip(gMsg);
    }
    else {
        EV_WARN << "Client " << clientId
                << ": Unknown message type - deleting" << endl;
        delete msg;
    }
}

void ClientNode::handleSubtask(SubtaskMsg *msg)
{
    int subtaskId = msg->getSubtaskId();
    int initiatorId = msg->getInitiatorId();
    int numElems = msg->getElementsArraySize();
    int hops = msg->getHopCount();

    // Compute maximum of the received sub-array
    int maxVal = msg->getElements(0);
    std::string elemStr = std::to_string(msg->getElements(0));
    for (int i = 1; i < numElems; i++) {
        int val = msg->getElements(i);
        maxVal = std::max(maxVal, val);
        elemStr += ", " + std::to_string(val);
    }

    logOutput("Client " + std::to_string(clientId) +
              ": Received subtask " + std::to_string(subtaskId) +
              " from initiator " + std::to_string(initiatorId) +
              " | Elements: [" + elemStr + "]" +
              " | Max: " + std::to_string(maxVal) +
              " | Hops: " + std::to_string(hops));

    // Send result back to the initiator
    SubtaskResultMsg *result = new SubtaskResultMsg("subtaskResult");
    result->setSubtaskId(subtaskId);
    result->setInitiatorId(initiatorId);
    result->setTargetClientId(initiatorId);
    result->setSourceClientId(clientId);
    result->setMaxValue(maxVal);
    result->setHopCount(0);

    delete msg;

    routeMessage(result, initiatorId);
}

void ClientNode::handleSubtaskResult(SubtaskResultMsg *msg)
{
    int subtaskId = msg->getSubtaskId();
    int maxVal = msg->getMaxValue();
    int hops = msg->getHopCount();

    subtaskResults[subtaskId] = maxVal;

    logOutput("Client " + std::to_string(clientId) +
              ": Result for subtask " + std::to_string(subtaskId) +
              " -> max = " + std::to_string(maxVal) +
              " (hops: " + std::to_string(hops) + ")" +
              " [" + std::to_string(subtaskResults.size()) + "/" +
              std::to_string(expectedResults) + "]");

    delete msg;

    // Check if all subtask results have been received
    if ((int)subtaskResults.size() == expectedResults) {
        // Compute the consolidated maximum
        int globalMax = subtaskResults.begin()->second;

        logOutput("");
        logOutput("============================================================");
        logOutput("Client " + std::to_string(clientId) +
                  ": ALL SUBTASK RESULTS RECEIVED");
        logOutput("============================================================");

        for (const auto &pair : subtaskResults) {
            globalMax = std::max(globalMax, pair.second);
            logOutput("  Subtask " + std::to_string(pair.first) +
                      ": max = " + std::to_string(pair.second));
        }

        logOutput("------------------------------------------------------------");
        logOutput("  >>> CONSOLIDATED RESULT: Maximum element = " +
                  std::to_string(globalMax) + " <<<");
        logOutput("============================================================");
        logOutput("");

        // Task complete - initiate gossip protocol
        taskCompleted = true;
        initiateGossip();
    }
}

// =====================================================================
//                       GOSSIP PROTOCOL
// =====================================================================

std::string ClientNode::computeHash(const std::string &input)
{
    // Simple hash using std::hash for gossip deduplication
    std::hash<std::string> hasher;
    size_t hashVal = hasher(input);
    std::stringstream ss;
    ss << std::hex << hashVal;
    return ss.str();
}

std::string ClientNode::getIPAddress()
{
    return "10.0.0." + std::to_string(clientId);
}

void ClientNode::initiateGossip()
{
    // Generate gossip message: <timestamp>:<IP>:<ClientID>
    std::string timestamp = std::to_string(simTime().dbl());
    std::string ip = getIPAddress();
    std::string msgContent = timestamp + ":" + ip + ":" + std::to_string(clientId);
    std::string hash = computeHash(msgContent);

    // Record own gossip in the message list
    messageList.insert(hash);
    gossipReceived.insert(clientId);

    logOutput("Client " + std::to_string(clientId) +
              ": GOSSIP GENERATED -> \"" + msgContent + "\"");

    // Broadcast gossip to all neighbors
    for (const auto &pair : neighborToGate) {
        GossipMsg *gMsg = new GossipMsg("gossip");
        gMsg->setOriginClientId(clientId);
        gMsg->setTimestamp(timestamp.c_str());
        gMsg->setIpAddress(ip.c_str());
        gMsg->setMsgContent(msgContent.c_str());
        gMsg->setSenderClientId(clientId);
        gMsg->setMsgHash(hash.c_str());

        send(gMsg, "out", pair.second);
    }

    checkTermination();
}

void ClientNode::handleGossip(GossipMsg *msg)
{
    std::string hash = msg->getMsgHash();
    int senderClientId = msg->getSenderClientId();
    int originClientId = msg->getOriginClientId();
    std::string msgContent = msg->getMsgContent();

    // Check if we've already seen this gossip message
    if (messageList.find(hash) != messageList.end()) {
        // Duplicate - ignore (prevents loops and redundant forwarding)
        EV << "Client " << clientId
           << ": Ignoring duplicate gossip from Client "
           << originClientId << endl;
        delete msg;
        return;
    }

    // First time receiving this gossip - record in ML
    messageList.insert(hash);
    gossipReceived.insert(originClientId);

    // Log the gossip receipt (required output)
    std::string logMsg = "[T=" + std::to_string(simTime().dbl()) +
                         "] Client " + std::to_string(clientId) +
                         " received gossip from Client " +
                         std::to_string(senderClientId) +
                         " (IP: " + getIPAddress() + ")" +
                         ": \"" + msgContent + "\"" +
                         " (origin: Client " + std::to_string(originClientId) +
                         ") [FIRST TIME]";
    logOutput(logMsg);

    // Forward to all neighbors EXCEPT the sender (gossip protocol)
    for (const auto &pair : neighborToGate) {
        if (pair.first != senderClientId) {
            GossipMsg *fwd = new GossipMsg("gossip");
            fwd->setOriginClientId(originClientId);
            fwd->setTimestamp(msg->getTimestamp());
            fwd->setIpAddress(msg->getIpAddress());
            fwd->setMsgContent(msgContent.c_str());
            fwd->setSenderClientId(clientId);  // We are the new sender
            fwd->setMsgHash(hash.c_str());

            send(fwd, "out", pair.second);
        }
    }

    delete msg;
    checkTermination();
}

void ClientNode::checkTermination()
{
    // Terminate when gossip has been received from ALL N clients
    if ((int)gossipReceived.size() == numClients && !allGossipReceived) {
        allGossipReceived = true;
        logOutput("");
        logOutput("************************************************************");
        logOutput("Client " + std::to_string(clientId) + ": TERMINATION");
        logOutput("Received gossip from all " + std::to_string(numClients) +
                  " clients. Terminating.");
        logOutput("************************************************************");
        logOutput("");
    }
}

// =====================================================================
//                           UTILITY
// =====================================================================

void ClientNode::logOutput(const std::string &text)
{
    // Output to both OMNeT++ event log and the shared output file
    EV << text << endl;
    if (outputFile.is_open()) {
        outputFile << text << std::endl;
    }
}

void ClientNode::finish()
{
    if (outputFile.is_open()) {
        outputFile.close();
    }

    EV << "Client " << clientId << " finished. "
       << "Task completed: " << (taskCompleted ? "YES" : "NO")
       << " | Gossip received: " << gossipReceived.size()
       << "/" << numClients << endl;
}
