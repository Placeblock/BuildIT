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
#include "simulation/simulation.h"
#include "mesh.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
private:
    const std::string text;
    Sim::Simulation* simulation;
    const std::shared_ptr<Sim::Node> simNode;
    static glm::vec2 calcSize(std::shared_ptr<Sim::Node> simNode);
protected:
    std::vector<glm::vec2> calculateInputCells() override;
    std::vector<glm::vec2> calculateOutputCells() override;
public:
    Gate(glm::vec2 cell, Mesh* mesh, std::string text, Sim::Simulation* simulation, std::shared_ptr<Sim::Node> simNode);
    void onInputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
};


#endif //BUILDIT_GATE_H
