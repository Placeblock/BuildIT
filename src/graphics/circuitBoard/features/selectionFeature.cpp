//
// Created by felix on 9/7/24.
//

#include "selectionFeature.h"

std::list<Component *> *SelectionFeature::getComponents() {
    return this->selection.getComponents();
}

void SelectionFeature::clearSelection() {
    this->selection.clear();
}

void SelectionFeature::addComponent(Component *component) {
    this->selection.addComponent(component);
}

void SelectionFeature::removeComponent(Component *component) {
    this->selection.removeComponent(component);
}

void SelectionFeature::notify(const HistoryChangeEvent &data) {
    this->clearSelection();
}
