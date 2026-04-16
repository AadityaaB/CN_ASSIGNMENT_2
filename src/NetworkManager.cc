// NetworkManager.cc
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

    std::set<std::pair<int,int>> edges;
    int a, b;
    while (file >> a >> b) {
        if (a < 0 || a >= N || b < 0 || b >= N) continue;
        edges.insert({std::min(a, b), std::max(a, b)});
    }
    file.close();

    // ---- Step 2: Compute Chord finger table edges ----
    int m = (N > 1) ? (int)ceil(log2((double)N)) : 0;
    for (int n = 0; n < N; n++) {
        for (int i = 0; i < m; i++) {
            int fingerTarget = (n + (1 << i)) % N;
            if (fingerTarget != n) {
                edges.insert({std::min(n, fingerTarget), std::max(n, fingerTarget)});
            }
        }
    }

    // ---- Step 3: Clear output file ----
    std::ofstream clearFile("outputfile.txt", std::ios::trunc);
    clearFile << "========================================" << std::endl;
    clearFile << "Chord P2P Network Simulation Output" << std::endl;
    clearFile << "========================================" << std::endl << std::endl;
    clearFile.close();

    // ---- Step 4: Dynamically create ClientNode modules ----
    cModuleType *clientType = cModuleType::get("chordp2p.ClientNode");
    network->setSubmoduleVectorSize("client", N);

    for (int i = 0; i < N; i++) {
        cModule *mod = clientType->create("client", network, i);
        mod->par("clientId") = i;
        mod->par("numClients") = N;
        mod->par("configFile") = configFile.c_str();

        double angle = 2.0 * M_PI * i / N - M_PI / 2.0;
        double radius = 250.0;
        int xPos = (int)(350 + radius * cos(angle));
        int yPos = (int)(350 + radius * sin(angle));

        mod->getDisplayString().setTagArg("p", 0, xPos);
        mod->getDisplayString().setTagArg("p", 1, yPos);
        mod->getDisplayString().setTagArg("i", 0, "device/pc");

        mod->finalizeParameters();
        mod->buildInside();
    }

    // ---- Step 5: Set gate sizes based on edge count ----
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
        ch1->setDelay(0.01);
        modA->gate("out", idxA)->connectTo(modB->gate("in", idxB), ch1);
        ch1->callInitialize(); // Initialize forward wire

        // Reverse link: B -> A
        cDelayChannel *ch2 = cDelayChannel::create("channel");
        ch2->setDelay(0.01);
        modB->gate("out", idxB)->connectTo(modA->gate("in", idxA), ch2);
        ch2->callInitialize(); // Initialize reverse wire
    }

    // ---- Step 7: Initialize all client modules ----
    for (int i = 0; i < N; i++) {
        cModule *mod = network->getSubmodule("client", i);
        mod->callInitialize();
    }
}

void NetworkManager::handleMessage(cMessage *msg)
{
    delete msg;
}