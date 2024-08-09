//
// Created by felix on 8/10/24.
//

#include "wiresAction.h"


void WiresAction::checkRegenerate() {
    if (this->regenerate) {
        this->renderer->regenerateData(&this->wires->vertices, &this->wires->wires);
    }
}
