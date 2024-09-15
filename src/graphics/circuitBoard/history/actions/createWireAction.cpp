//
// Created by felix on 8/3/24.
//

#include "createWireAction.h"

void CreateWireAction::execute(bool lastInBatch) {
    this->wire->connect();
    this->wireContainer->addWire(this->wire);
}

void CreateWireAction::rewind(bool lastInBatch) {
    this->wire->disconnect();
    this->wireContainer->removeWire(this->wire.get());
}
