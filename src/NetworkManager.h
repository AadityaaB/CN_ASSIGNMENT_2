//============================================================
// NetworkManager.h
// Reads topo.txt, dynamically creates ClientNode modules,
// computes Chord finger table edges, and establishes all
// network connections (ring + finger table shortcuts).
//============================================================

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <omnetpp.h>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace omnetpp;

class NetworkManager : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
