//
// Created by felix on 8/3/24.
//

#include "createWireAction.h"

void CreateWireAction::execute(bool lastInBatch) {
    this->wireContainer->addWire(this->wire);
}

void CreateWireAction::rewind(bool lastInBatch) {
    this->wireContainer->removeWire(this->wire.get());
}
