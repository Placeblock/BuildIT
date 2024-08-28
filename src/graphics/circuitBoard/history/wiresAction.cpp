//
// Created by felix on 8/10/24.
//

#include "wiresAction.h"


void WiresAction::regenerate() {
    std::set<const Joint*> nOJoints = this->wires->getJoints();
    std::set<const Wire*> nOWires = this->wires->getWires();
    this->renderer->regenerateData(nOJoints, nOWires);
}
