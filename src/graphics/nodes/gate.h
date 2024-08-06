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
#include "simulation/node.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
private:
    const std::string text;
    const std::shared_ptr<Sim::Node> simNode;
public:
    const int inputs;
    const int outputs;
    void onInputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
};

class Gates {
    std::set<std::shared_ptr<Gate>> gates;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> cellMap;
    std::unordered_map<glm::vec2, std::shared_ptr<Gate>> connectorsMap;
    //std::shared_ptr<Gate> searchGate(glm::vec2 cell);
};


#endif //BUILDIT_GATE_H
