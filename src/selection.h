#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include "algorithm/algorithm.h"

class Selection {
private:
    std::set<Node*> gates;
public:
    std::set<Node*> getGates();
    std::set<Node*> getSorted(); // Returns the gates with stubs first
};

#endif //BUILDIT_SELECTION_H
