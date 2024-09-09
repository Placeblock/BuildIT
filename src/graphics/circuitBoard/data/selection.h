//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include <unordered_set>
#include "graphics/circuitBoard/components/abstraction/component.h"

class Selection {
private:
    std::list<Component*> components;
public:
    void clear();
    void addComponent(Component *component);
    void removeComponent(Component *component);
    std::list<Component*>* getComponents();
};


#endif //BUILDIT_SELECTION_H
