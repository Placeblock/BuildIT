//
// Created by felix on 9/16/24.
//

#include "selectable.h"

void Selectable::select() {
    if (this->selected) return;
    this->selected = true;
    this->Subject<SelectEvent>::notify({this});
}

void Selectable::deselect() {
    if (!this->selected) return;
    this->selected = false;
    this->Subject<DeselectEvent>::notify({this});
}

bool Selectable::isSelected() const {
    return this->selected;
}

Selectable::Selectable(std::string cnamespace, std::string ckey) : Component(std::move(cnamespace), std::move(ckey)) {

}
