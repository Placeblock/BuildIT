//
// Created by felix on 8/9/24.
//

#include "selection.h"
#include "graphics/circuitBoard/selection/selectable.h"

void Selection::clearSelection() {
    while (!this->selected.empty()) {
        this->deselect(*this->selected.begin());
    }
}

std::list<Selectable*> *Selection::getComponents() {
    return &this->selected;
}

void Selection::select(Selectable *selectable) {
    this->selected.push_back(selectable);
    selectable->select();
}

void Selection::deselect(Selectable *selectable) {
    this->selected.remove(selectable);
    selectable->deselect();
}
