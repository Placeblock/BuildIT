The history directory contains all action-related logic.
To support undo and rewind, everything that can modify the circuit board
has to do it through specific Actions.

These Actions can be executed and rewind. When executing the actions through
the History class they get stored inside the undo deque and redo stack to
support undo and redo.

Everything Component (e.g. Node or Wire) that should only be discarded (deleted from memory)
if it is not in the undo deque or redo stack anymore should use shared pointers to manage ownership.