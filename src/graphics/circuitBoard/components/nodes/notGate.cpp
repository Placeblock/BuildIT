//
// Created by felix on 9/10/24.
//

#include "notGate.h"

#include <utility>


NotGate::NotGate(glm::vec2 pos, std::string text, const std::shared_ptr<Sim::Node> &simNode)
    : Gate(pos, std::move(text), simNode) {

}

void NotGate::visit(Visitor *visitor) {
    visitor->doFor(this);
}

NotGate::NotGate(NotGate &other) {

}
