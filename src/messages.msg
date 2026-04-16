//============================================================
// messages.msg
// Defines all message types exchanged between ClientNodes:
//   - SubtaskMsg:       Carries a subtask (sub-array) to a target client
//   - SubtaskResultMsg: Returns the computed max back to the initiator
//   - GossipMsg:        Gossip announcement after task completion
//============================================================

// SubtaskMsg: Sent from initiator to target client via Chord routing.
// Contains the sub-array of elements for which the target must find the max.
message SubtaskMsg
{
    int subtaskId;          // Unique subtask identifier
    int initiatorId;        // Client that originated this task
    int targetClientId;     // Destination client (subtaskId % N)
    int sourceClientId;     // Last forwarding client (for routing)
    int elements[];         // Dynamic array of elements in this subtask
    int hopCount=0;       // Number of hops taken so far
}

// SubtaskResultMsg: Sent from the processing client back to the initiator.
// Contains the maximum value found in the subtask's sub-array.
message SubtaskResultMsg
{
    int subtaskId;          // Corresponding subtask ID
    int initiatorId;        // Original initiator (for identification)
    int targetClientId;     // Destination (=initiatorId, routed back)
    int sourceClientId;     // Last forwarding client
    int maxValue;           // Computed maximum of the sub-array
    int hopCount=0;       // Number of hops taken so far
}

// GossipMsg: Broadcast after a client completes its entire task.
// Format: <timestamp>:<IP>:<ClientID>
message GossipMsg
{
    int originClientId;     // Client that generated this gossip
    string timestamp;       // Simulation time when gossip was generated
    string ipAddress;       // Simulated IP of the origin client
    string msgContent;      // Full gossip string: "timestamp:IP:clientId"
    int senderClientId;     // Last hop sender (to avoid reverse forwarding)
    string msgHash;         // Hash of msgContent for deduplication (ML)
}
