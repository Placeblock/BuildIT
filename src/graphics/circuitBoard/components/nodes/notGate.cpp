//
// Created by felix on 9/10/24.
//

#include "notGate.h"

#include <utility>


NotGate::NotGate(glm::vec2 pos, const std::shared_ptr<Sim::Node> &simNode)
    : Gate(pos, "!", simNode) {

}

NotGate::NotGate(NotGate &other) : Gate(other) {

}
