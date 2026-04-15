// ============================================================
// ChordNetwork.ned
// Defines all module types and the top-level network for the
// Chord-based P2P distributed computation simulation.
// ============================================================

package chordp2p;

//
// ClientNode: A peer in the Chord ring that can initiate tasks,
// process subtasks, return results, and participate in gossip.
//
simple ClientNode
{
    parameters:
        int clientId = default(-1);
        int numClients = default(0);
        string configFile = default("");
        @display("i=device/pc");
    gates:
        input in[];
        output out[];
}

//
// NetworkManager: Reads the topology file, dynamically creates
// ClientNode modules, computes Chord finger table connections,
// and wires up the network.
//
simple NetworkManager
{
    parameters:
        @display("i=block/control");
}

//
// ChordNetwork: The top-level simulation network.
// Only contains the NetworkManager; client nodes are created
// dynamically by the manager based on topo.txt.
//
network ChordNetwork
{
    parameters:
        string topoFile = default("topo.txt");
        string configFile = default("config.txt");
        @display("bgb=700,700");
    submodules:
        manager: NetworkManager;
    // Client nodes are created dynamically by NetworkManager
}
