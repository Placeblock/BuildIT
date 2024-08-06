//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_GATE_H
#define BUILDIT_GATE_H

#include <string>
#include <set>
#include <memory>
#include "node.h"
#include "glm/gtx/hash.hpp"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
public:
    const std::string text;
    const int inputs;
    const int outputs;
};

class Gates {
    std::set<std::shared_ptr<Gate>> gates;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> cellMap;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> connectorsMap;
    //std::shared_ptr<Gate> searchGate(glm::vec2 cell);
};


#endif //BUILDIT_GATE_H
