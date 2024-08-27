//
// Created by felix on 8/10/24.
//

#include "wiresAction.h"


void WiresAction::regenerate() {
    std::set<const Joint*> nOJoints = this->wires->getNonOwningJoints();
    std::set<const Wire*> nOWires = this->wires->getNonOwningWires();
    this->renderer->regenerateData(nOJoints, nOWires);
}
