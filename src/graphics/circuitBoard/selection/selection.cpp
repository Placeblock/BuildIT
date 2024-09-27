//
// Created by felix on 8/9/24.
//

#include <algorithm>
#include <utility>
#include "selection.h"
#include "graphics/circuitBoard/selection/selectable.h"

void Selection::clearSelection() {
    while (!this->selected.empty()) {
        this->deselect(*this->selected.begin());
    }
}

std::list<Selectable*> *Selection::getSelected() {
    return &this->selected;
}

void Selection::select(Selectable *selectable) {
    if (this->isSelected(selectable)) return;
    this->selected.push_back(selectable);
    selectable->select();
}

void Selection::deselect(Selectable *selectable) {
    if (!this->isSelected(selectable)) return;
    this->selected.remove(selectable);
    selectable->deselect();
}

void Selection::deselectIf(std::function<bool(Selectable*)> predicate) {
    this->selected.remove_if([&predicate](Selectable *selected) {
        if (predicate(selected)) {
            selected->deselect();
            return true;
        }
        return false;
    });
}

bool Selection::isSelected(Selectable *selectable) const {
    return std::find(this->selected.begin(), this->selected.end(), selectable) != this->selected.end();
}
