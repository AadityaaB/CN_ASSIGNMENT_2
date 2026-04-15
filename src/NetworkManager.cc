// ============================================================
// NetworkManager.cc
// Implementation of the NetworkManager module.
//
// Responsibilities:
// 1. Read topology from topo.txt (N nodes + ring edges)
// 2. Compute Chord finger table edges for O(log N) routing
// 3. Dynamically create N ClientNode modules
// 4. Establish all bidirectional connections (ring + finger)
// 5. Initialize all client modules
// ============================================================

#include "NetworkManager.h"

Define_Module(NetworkManager);

void NetworkManager::initialize()
{
    cModule *network = getParentModule();
    std::string topoFile = network->par("topoFile").stdstringValue();
    std::string configFile = network->par("configFile").stdstringValue();

    // ---- Step 1: Read topology file ----
    std::ifstream file(topoFile);
    if (!file.is_open()) {
        throw cRuntimeError("Cannot open topology file: %s", topoFile.c_str());
    }

    int N;
    file >> N;
    if (N <= 0) {
        throw cRuntimeError("Invalid number of nodes in topology: %d", N);
    }

    // Read ring edges from topology file
    std::set<std::pair<int,int>> edges;
    int a, b;
    while (file >> a >> b) {
        if (a < 0 || a >= N || b < 0 || b >= N) {
            EV_WARN << "Ignoring invalid edge: " << a << " - " << b << endl;
            continue;
        }
        edges.insert({std::min(a, b), std::max(a, b)});
    }
    file.close();

    EV << "========================================" << endl;
    EV << "NetworkManager: Read topology file" << endl;
    EV << "  Nodes: " << N << endl;
    EV << "  Ring edges: " << edges.size() << endl;

    // ---- Step 2: Compute Chord finger table edges ----
    // For each node n, finger[i] = (n + 2^i) % N, for i = 0..ceil(log2(N))-1
    int m = (N > 1) ? (int)ceil(log2((double)N)) : 0;
    EV << "  Finger table size (m): " << m << endl;

    for (int n = 0; n < N; n++) {
        EV << "  Node " << n << " fingers: [";
        for (int i = 0; i < m; i++) {
            int fingerTarget = (n + (1 << i)) % N;
            if (i > 0) EV << ", ";
            EV << fingerTarget;
            if (fingerTarget != n) {
                edges.insert({std::min(n, fingerTarget), std::max(n, fingerTarget)});
            }
        }
        EV << "]" << endl;
    }

    EV << "  Total unique edges (ring + finger): " << edges.size() << endl;
    EV << "========================================" << endl;

    // ---- Step 3: Clear and initialize output file ----
    std::ofstream clearFile("outputfile.txt", std::ios::trunc);
    clearFile << "========================================" << std::endl;
    clearFile << "Chord P2P Network Simulation Output" << std::endl;
    clearFile << "Number of clients: " << N << std::endl;
    clearFile << "Finger table size: " << m << std::endl;
    clearFile << "Total connections: " << edges.size() << std::endl;
    clearFile << "========================================" << std::endl;
    clearFile << std::endl;
    clearFile.close();

    // ---- Step 4: Dynamically create ClientNode modules ----
    cModuleType *clientType = cModuleType::get("chordp2p.ClientNode");

    for (int i = 0; i < N; i++) {
        cModule *mod = clientType->create("client", network, N, i);
        mod->par("clientId") = i;
        mod->par("numClients") = N;
        mod->par("configFile") = configFile.c_str();

        // Arrange nodes in a ring layout for Qtenv visualization
        double angle = 2.0 * M_PI * i / N - M_PI / 2.0;
        double radius = 250.0;
        int xPos = (int)(350 + radius * cos(angle));
        int yPos = (int)(350 + radius * sin(angle));

        mod->getDisplayString().setTagArg("p", 0, xPos);
        mod->getDisplayString().setTagArg("p", 1, yPos);
        mod->getDisplayString().setTagArg("i", 0, "device/pc");

        mod->finalizeParameters();
        mod->buildInside();

        EV << "Created Client " << i << " at position (" << xPos << "," << yPos << ")" << endl;
    }

    // ---- Step 5: Set gate sizes based on edge count per node ----
    std::map<int, int> edgeCount;
    for (int i = 0; i < N; i++) edgeCount[i] = 0;
    for (const auto &e : edges) {
        edgeCount[e.first]++;
        edgeCount[e.second]++;
    }

    for (int i = 0; i < N; i++) {
        cModule *mod = network->getSubmodule("client", i);
        mod->setGateSize("out", edgeCount[i]);
        mod->setGateSize("in", edgeCount[i]);
    }

    // ---- Step 6: Create bidirectional connections ----
    std::map<int, int> gateIdx;
    for (int i = 0; i < N; i++) gateIdx[i] = 0;

    for (const auto &e : edges) {
        cModule *modA = network->getSubmodule("client", e.first);
        cModule *modB = network->getSubmodule("client", e.second);

        int idxA = gateIdx[e.first]++;
        int idxB = gateIdx[e.second]++;

        // Forward link: A -> B
        cDelayChannel *ch1 = cDelayChannel::create("channel");
        ch1->setDelay(0.01);  // 10ms propagation delay
        modA->gate("out", idxA)->connectTo(modB->gate("in", idxB), ch1);

        // Reverse link: B -> A
        cDelayChannel *ch2 = cDelayChannel::create("channel");
        ch2->setDelay(0.01);
        modB->gate("out", idxB)->connectTo(modA->gate("in", idxA), ch2);
    }

    // ---- Step 7: Log final connectivity ----
    EV << endl << "=== Network Connectivity ===" << endl;
    for (int i = 0; i < N; i++) {
        cModule *mod = network->getSubmodule("client", i);
        std::string neighbors = "  Client " + std::to_string(i) + " <-> [";
        int numGates = mod->gateSize("out");
        bool first = true;
        for (int g = 0; g < numGates; g++) {
            cGate *outGate = mod->gate("out", g);
            if (outGate->isConnected()) {
                cGate *nextGate = outGate->getNextGate();
                if (nextGate) {
                    int neighborId = nextGate->getOwnerModule()->par("clientId");
                    if (!first) neighbors += ", ";
                    neighbors += std::to_string(neighborId);
                    first = false;
                }
            }
        }
        neighbors += "]";
        EV << neighbors << endl;
    }
    EV << "============================" << endl << endl;

    // ---- Step 8: Initialize all client modules ----
    for (int i = 0; i < N; i++) {
        cModule *mod = network->getSubmodule("client", i);
        mod->callInitialize();
    }

    EV << "NetworkManager: Network setup complete with " << N << " clients." << endl;
}

void NetworkManager::handleMessage(cMessage *msg)
{
    // NetworkManager does not process runtime messages
    delete msg;
}
