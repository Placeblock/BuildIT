//
// Created by felix on 9/10/24.
//

#include "notGate.h"

#include "graphics/constants.h"

const std::string KEY = "GATE:NOT";

NotGate::NotGate(glm::vec2 pos, const std::shared_ptr<Sim::Node> &simNode)
    : Gate(Constants::NAMESPACE, KEY, pos, "!", simNode),
        Component( Constants::NAMESPACE, KEY) {

}