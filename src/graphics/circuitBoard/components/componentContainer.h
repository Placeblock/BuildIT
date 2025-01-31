//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_COMPONENTCONTAINER_H
#define BUILDIT_COMPONENTCONTAINER_H

#include <memory>

#include "graphics/circuitBoard/components/abstraction/component.h"
#include <unordered_set>

#include "graphics/circuitBoard/observer.h"

class ComponentContainer : public Subject<ComponentAddEvent>, public Subject<ComponentRemoveEvent> {
public:
    virtual std::unordered_set<std::shared_ptr<Component>>* getComponents() = 0;
    virtual std::shared_ptr<Component> getOwningRef(Component* component) = 0;
    virtual void addComponent(std::shared_ptr<Component> component) = 0;
    virtual void removeComponent(Component *component) = 0;
};

#endif //BUILDIT_COMPONENTCONTAINER_H
