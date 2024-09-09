//
// Created by felix on 9/9/24.
//

#include "components.h"

std::unordered_set<std::shared_ptr<Component>>* Components::getComponents() {
    return &this->components;
}

void Components::addComponent(std::shared_ptr<Component> component) {
    this->components.insert(component);
    this->Subject<ComponentAddEvent>::notify({component.get()});
}

void Components::removeComponent(Component *component) {
    std::erase_if(this->components, [&component](const std::shared_ptr<Component>& comp){
        return comp.get() == component;
    });
    this->Subject<ComponentRemoveEvent>::notify({component});
}
