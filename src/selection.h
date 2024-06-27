#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include "gate.h"

class Selection {
protected:
    std::vector<Gate*> gates;
public:
    std::vector<Gate*> getGates();
    std::vector<Gate*> getReversedGates(); // Returns the gates in reverse order of the "tree"
};

#endif //BUILDIT_SELECTION_H
