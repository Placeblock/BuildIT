//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_GATE_H
#define BUILDIT_GATE_H

#include "wires/wires.h"

class Gate : public Node {
public:
    const glm::vec2 size;
    const std::string text;
    const int inputs;
    const int outputs;
    std::unordered_set<std::shared_ptr<Vertex>> connectedVertices;
    std::unordered_set<glm::vec2> calculatePinCells();
};

class Gates {
    std::set<std::shared_ptr<Vertex>> gates;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> cellMap;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> connectorsMap;
    std::shared_ptr<Gate> searchGate(glm::vec2 cell);
};


#endif //BUILDIT_GATE_H
