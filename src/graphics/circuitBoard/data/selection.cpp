//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    while (!this->components.empty()) {
        this->removeComponent(*this->components.begin());
    }
}

void Selection::addComponent(const Component *component) {
    this->components.insert(component);
}

void Selection::removeComponent(const Component *component) {
    this->components.erase(component);
}
