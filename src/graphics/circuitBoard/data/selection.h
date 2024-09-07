//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include <unordered_set>
#include "graphics/circuitBoard/components/component.h"

class Selection {
private:
    std::unordered_set<const Component*> components;
public:
    void clear();
    void addComponent(const Component *component);
    void removeComponent(const Component *component);
};


#endif //BUILDIT_SELECTION_H
