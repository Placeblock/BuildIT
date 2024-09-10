//
// Created by felix on 9/10/24.
//

#include "notGate.h"

#include <utility>


void NotGate::visit(Visitor *visitor) {
    visitor->doFor(this);
}

NotGate::NotGate(glm::vec2 cell, std::string text, const std::shared_ptr<Sim::Node> &simNode)
    : Gate(cell, std::move(text), simNode) {

}
