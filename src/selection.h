#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include "algorithm.h"

class Selection {
private:
    std::set<Gate*> gates;
public:
    std::set<Gate*> getGates();
    std::set<Gate*> getSorted(); // Returns the gates with stubs first
};

#endif //BUILDIT_SELECTION_H
