//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    while (!this->components.empty()) {
        this->removeComponent(*this->components.begin());
    }
}

std::list<Component *> *Selection::getComponents() {
    return &this->components;
}

void Selection::addComponent(Component *component) {
    this->components.push_back(component);
}

void Selection::removeComponent(Component *component) {
    this->components.remove(component);
}
