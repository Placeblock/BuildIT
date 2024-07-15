#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include "algorithm/algorithm.h"

class Selection {
private:
    std::set<Node*> nodes;
public:
    std::set<Node*> getNodes();
    void addNode(Node* node);
};

#endif //BUILDIT_SELECTION_H
