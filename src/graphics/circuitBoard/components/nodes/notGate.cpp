//
// Created by felix on 9/10/24.
//

#include "notGate.h"

#include <utility>


void NotGate::visit(Visitor *visitor) {
    visitor->doFor(this);
}

NotGate::NotGate(glm::vec2 pos, std::string text, const std::shared_ptr<Sim::Node> &simNode)
    : Gate(pos, std::move(text), simNode), Movable(pos, Gate::calcSize(simNode)*32),
      Positionable(pos, Gate::calcSize(simNode)*32) {

}
