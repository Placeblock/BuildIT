//
// Created by felix on 9/9/24.
//

#include <algorithm>
#include <cassert>
#include "components.h"

std::unordered_set<std::shared_ptr<Component>>* Components::getComponents() {
    return &this->components;
}

void Components::addComponent(const std::shared_ptr<Component> component) {
    this->components.insert(component);
    this->Subject<ComponentAddEvent>::notify({component.get()});
}

void Components::removeComponent(Component *component) {
    std::erase_if(this->components, [&component](const std::shared_ptr<Component>& comp){
        return comp.get() == component;
    });
    this->Subject<ComponentRemoveEvent>::notify({component});
}

std::shared_ptr<Component> Components::getOwningRef(Component *component) {
    const auto iter = std::find_if(this->components.begin(), this->components.end(), [&component](const std::shared_ptr<Component>& c){
        return c.get() == component;
    });
    if (iter == this->components.end()) return nullptr;
    return *iter;
}
