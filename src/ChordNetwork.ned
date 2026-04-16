package chordp2p;

// 1. Define the blueprint for a ClientNode
simple ClientNode
{
    parameters:
        int clientId;
        int numClients;
        string configFile;
        @display("i=device/pc");
    gates:
        input in[];
        output out[];
}

// 2. Define the blueprint for the NetworkManager
simple NetworkManager
{
    parameters:
        @display("i=block/cogwheel");
}

// 3. Define the overall Network that holds them together
network ChordNetwork
{
    parameters:
        string topoFile;
        string configFile;
    submodules:
        manager: NetworkManager;

        // This is the magic line! We declare an empty vector of clients
        // so our C++ code can dynamically resize and populate it based on topo.txt
        client[0]: ClientNode;
}
