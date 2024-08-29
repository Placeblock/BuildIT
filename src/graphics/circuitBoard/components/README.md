The simBridge directory contains everything that can be put on the circuit board.

It includes code for nodes, e.g. gates, wires and joints.

The Wires class manages multiple wires and joins. It implements the pure virtual
JointContainer class which contains methods for adding, removing and moving joints.

The Nodes class manages multiple nodes, e.g. Gates. It implements the pure virtual
NodeContainer class which contains methods for adding, removing and moving nodes.

It also contains the SimulationBridge, which uses both Nodes and Wires to tie the
two together. It synchronizes the simulation when nodes or joints are moved around,
added or removed.