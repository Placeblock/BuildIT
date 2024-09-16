//
// Created by felix on 9/16/24.
//

#include "selectable.h"

void Selectable::select() {
    this->Subject<SelectEvent>::notify({this});
}

void Selectable::deselect() {
    this->Subject<DeselectEvent>::notify({this});
}
