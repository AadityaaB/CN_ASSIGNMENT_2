// ============================================================
// ClientNode.h
// Header for the ClientNode module - a peer in the Chord P2P
// network that can initiate tasks, compute subtask results,
// and participate in gossip-based termination.
// ============================================================

#ifndef CLIENTNODE_H_
#define CLIENTNODE_H_

#include <omnetpp.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <functional>
#include <algorithm>

using namespace omnetpp;

// Forward declarations for generated message classes
class SubtaskMsg;
class SubtaskResultMsg;
class GossipMsg;

class ClientNode : public cSimpleModule
{
  protected:
    // ---- Identity ----
    int clientId;
    int numClients;

    // ---- Chord Routing ----
    std::vector<int> fingerTable;           // finger[i] = (clientId + 2^i) % N
    std::map<int, int> neighborToGate;      // neighborClientId -> output gate index
    int successor;
    int predecessor;

    // ---- Task Management ----
    std::vector<int> fullArray;             // The complete array for the task
    int numSubtasks;                        // x: number of subtasks
    int arraySize;                          // k: array length
    std::map<int, int> subtaskResults;      // subtaskId -> max value received
    int expectedResults;                    // Total subtask results expected
    bool taskCompleted;

    // ---- Gossip Protocol ----
    std::set<std::string> messageList;      // ML: hashes of seen gossip messages
    std::set<int> gossipReceived;           // Set of client IDs whose gossip we've seen
    bool allGossipReceived;

    // ---- Output ----
    std::ofstream outputFile;

    // ---- Configuration ----
    std::string configFile;

    // ---- Lifecycle ----
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // ---- Configuration ----
    void readConfig();

    // ---- Chord Routing ----
    void buildFingerTable();
    void buildNeighborMap();
    int findNextHop(int targetId);
    void routeMessage(cMessage *msg, int targetId);
    bool isInOpenInterval(int x, int a, int b);
    bool isInHalfOpenInterval(int x, int a, int b);

    // ---- Task Handling ----
    void divideAndDistribute();
    void handleSubtask(SubtaskMsg *msg);
    void handleSubtaskResult(SubtaskResultMsg *msg);

    // ---- Gossip Protocol ----
    void initiateGossip();
    void handleGossip(GossipMsg *msg);
    void checkTermination();

    // ---- Utility ----
    std::string computeHash(const std::string &input);
    std::string getIPAddress();
    void logOutput(const std::string &text);
};

#endif
