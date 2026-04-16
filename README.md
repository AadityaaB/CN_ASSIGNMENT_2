# Chord P2P Network Simulation - OMNeT++ 6.3.0

## Overview

This project implements a **Chord-based peer-to-peer (P2P) network simulation** using the OMNeT++ 6.3.0 Discrete Event Simulator. The network consists of **N client nodes** arranged in a **ring topology** augmented with **Chord finger table connections** for efficient O(log N) message routing.

Each client independently executes a distributed computation task — **finding the maximum element in an array** — by dividing the array into `x` subtasks and distributing them across the network. After task completion, clients use a **gossip protocol** to announce their results and gracefully terminate.

---

## Architecture

### Network Topology

```
           Client 0
          /    |    \
    Client 7   |   Client 1
      |    \   |   /    |
      |   Client 2     |
    Client 6    |   Client 3
      |    \   |   /    |
       \    Client 4   /
         \    |    /
          Client 5
```

- **Ring Connections**: Each client is connected to its successor and predecessor (from `topo.txt`)
- **Finger Table Connections**: Additional Chord shortcuts for O(log N) routing
  - `finger[i]=(clientId + 2^i) % N` for `i=0, 1, ..., ⌈log₂(N)⌉ - 1`

### Chord Finger Table Example (N=8)

| Client | finger[0] | finger[1] | finger[2] |
|--------|-----------|-----------|-----------|
| 0      | 1         | 2         | 4         |
| 1      | 2         | 3         | 5         |
| 2      | 3         | 4         | 6         |
| 3      | 4         | 5         | 7         |
| 4      | 5         | 6         | 0         |
| 5      | 6         | 7         | 1         |
| 6      | 7         | 0         | 2         |
| 7      | 0         | 1         | 3         |

---

## How It Works

### 1. Network Setup (NetworkManager)
1. Reads `topo.txt` to determine N (number of clients) and ring edges
2. Computes Chord finger table for each node
3. Dynamically creates N `ClientNode` modules
4. Establishes all connections (ring + finger table shortcuts)

### 2. Task Distribution (Each Client)
1. Reads `config.txt` to get the array of `k` elements and subtask count `x`
2. Divides the array into `x` approximately equal parts (each with ≥ 2 elements)
3. Assigns subtask `i` to client `i % N`
4. Routes each subtask to the target client using **Chord routing** (O(log N) hops)

### 3. Subtask Processing
1. Target client receives the sub-array
2. Computes the local maximum
3. Routes the result back to the initiator via Chord

### 4. Result Consolidation
1. Initiator collects all `x` subtask results
2. Computes the global maximum (max of all partial maxes)

### 5. Gossip Protocol
1. After task completion, each client generates: `<timestamp>:<IP>:<ClientID>`
2. Broadcasts to all neighbors
3. On first receipt: record hash in ML, forward to all except sender
4. Duplicates are ignored (prevents loops)

### 6. Termination
- Each client terminates after receiving gossip from **all N clients**

---

## Project Structure

```
CN_ASSIGNMENT_2/
├── src/
│   ├── package.ned           # NED package declaration
│   ├── ChordNetwork.ned      # Module and network definitions
│   ├── messages.msg           # Message type definitions
│   ├── NetworkManager.h       # Network setup module header
│   ├── NetworkManager.cc      # Network setup implementation
│   ├── ClientNode.h           # Client node header
│   └── ClientNode.cc          # Client node implementation (core logic)
├── simulations/
│   ├── omnetpp.ini            # Simulation configuration
│   ├── config.txt             # Array and subtask configuration
│   └── topo.txt               # Network topology (editable)
├── .nedfolders                # NED source directory listing
└── README.md                  # This file
```

---

## Configuration Files

### `topo.txt` — Network Topology

```
N
node_a node_b
node_a node_b
...
```

- **Line 1**: Number of nodes (N)
- **Remaining lines**: Undirected ring edges (one per line)
- Finger table connections are computed **automatically** by the program
- **Editable during evaluation** — just change N and list the ring edges

Example for N=4:
```
4
0 1
1 2
2 3
3 0
```

### `config.txt` — Task Configuration

```
k=<array_size>
x=<num_subtasks>
array=<comma_separated_values>
```

- `k`: Total number of elements in the array
- `x`: Number of subtasks (must satisfy `x>N` and `k/x>=2`)
- `array`: Comma-separated list of `k` integers

---
## Build & Run Instructions

### Option A: Native macOS / Linux Terminal (High-Performance)
This is the recommended approach for running the simulation natively on UNIX-based systems (like macOS or Linux), allowing you to leverage multi core compilation.

1. **Initialize the OMNeT++ Environment**:
   Open your terminal and navigate to your OMNeT++ installation directory to source the environment variables.
```bash
   cd /path/to/omnetpp-6.3.0
   source setenv
```

2. **Navigate to the Source Directory**:
```bash
   cd /path/to/CN_ASSIGNMENT_2/src
```

3. **Generate the Makefile and Compile**: Use `opp_makemake` to link the files, and compile using multiple cores (e.g., `-j8` for an 8-core processor like Apple Silicon).
```bash
   opp_makemake -f --deep -o chordp2p
   make -j8
```

4. **Run the Simulation**: Navigate to the simulations folder and execute the binary in command-line mode.
```bash
   cd ../simulations
   ../src/chordp2p -f omnetpp.ini -u Cmdenv -n ../src
```

---

### Option B: Using OMNeT++ IDE (GUI Method)

1. Open OMNeT++ 6.3.0 IDE.
2. Import the project:
   - `File` → `Open Projects from File System...`
   - Select the `CN_ASSIGNMENT_2` directory and click `Finish`.
3. Build the project:
   - Right-click the project → `Build Project` (or press `Ctrl+B`).
   - The build system automatically processes `.msg` files and compiles all C++ code.
4. Run the simulation:
   - Open `simulations/omnetpp.ini`.
   - Click `Run` → `Run As` → `OMNeT++ Simulation`.
   - Choose `Cmdenv` for command-line output or `Qtenv` for the visual GUI.

---

### Option C: Using Command Line (Windows MinGW Shell)

1. Open the OMNeT++ MinGW terminal (from the OMNeT++ installation folder).
2. Navigate to the project directory:
```bash
   cd /path/to/CN_ASSIGNMENT_2/src
```
3. Generate the Makefile and Build:
```bash
   opp_makemake -f --deep -o chordp2p
   make
```
4. Run the simulation:
```bash
   cd ../simulations
   ../src/chordp2p -f omnetpp.ini -u Cmdenv -n ../src
```

---

## Output

### Console Output
All output is displayed on the console via OMNeT++ `EV` logging.

### Output File (`outputfile.txt`)
Generated in the `simulations/` directory at runtime. Contains:
- Task initiation details (array, subtask assignment)
- Subtask processing results (elements, local max, hop count)
- Consolidated results (global maximum)
- Gossip messages (with timestamps and sender info)
- Termination notices

### Sample Output
```
============================================================
Client 0: TASK INITIATION
============================================================
Array (32 elements): [45, 12, 78, 3, 56, 89, 23, 67, ...]
Subtasks (x): 12 | Clients (N): 8

  Subtask 0 -> Client 0: [45, 12, 78] [LOCAL]
  Subtask 1 -> Client 1: [3, 56, 89] [REMOTE]
  ...

Client 3: Received subtask 3 from initiator 0 | Elements: [67, 34, 90] | Max: 90 | Hops: 1

Client 0: Result for subtask 3 -> max=90 (hops: 1) [4/12]

============================================================
Client 0: ALL SUBTASK RESULTS RECEIVED
============================================================
  Subtask 0: max=78
  Subtask 1: max=89
  ...
------------------------------------------------------------
  >>> CONSOLIDATED RESULT: Maximum element=99 <<<
============================================================

Client 0: GOSSIP GENERATED -> "5.5:10.0.0.0:0"

[T=5.52] Client 1 received gossip from Client 0 (IP: 10.0.0.1): "5.5:10.0.0.0:0" (origin: Client 0) [FIRST TIME]

************************************************************
Client 0: TERMINATION
Received gossip from all 8 clients. Terminating.
************************************************************
```

---

## Changing Network Size (for evaluation)

1. **Edit `simulations/topo.txt`**:
   - Change the first line to the new N
   - List ring edges: `0 1`, `1 2`, ..., `(N-1) 0`

2. **Edit `simulations/config.txt`** (if needed):
   - Ensure `x>N` and `k/x>=2`

3. **Rebuild and re-run** — no code changes needed!

---

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Dynamic module creation** | N is determined entirely by `topo.txt` — no need to hardcode in NED or .ini |
| **Chord finger table** | Provides O(log N) routing instead of O(N) ring traversal |
| **Direct-connection optimization** | If the target is a direct neighbor, skip finger table lookup |
| **Staggered task start** | Clients start 0.5s apart to produce cleaner, readable output |
| **Shared output file** | All clients append to one `outputfile.txt` for unified logging |

---

## Contributors

|       Name     | Roll Number |
|----------------|-------------|
| Aaditya Bansal |  B23CS1083  |
| Saher Dev      |  B23CS1059  |
