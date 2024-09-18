//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_COMPONENTS_H
#define BUILDIT_COMPONENTS_H


#include "componentContainer.h"

class Components final : public ComponentContainer {
    std::unordered_set<std::shared_ptr<Component>> components;
public:
    std::unordered_set<std::shared_ptr<Component>>* getComponents() override;
    void addComponent(std::shared_ptr<Component> component) override;
    void removeComponent(Component *component) override;
    std::shared_ptr<Component> getOwningRef(Component* component) override;
};


#endif //BUILDIT_COMPONENTS_H
