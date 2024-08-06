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
#include "graphics/renderer/meshRenderer.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
private:
    MeshRenderer* mesh;
    const std::string text;
    Sim::Simulation* simulation;
    const std::shared_ptr<Sim::Node> simNode;
    static glm::vec2 calcSize(std::shared_ptr<Sim::Node> simNode);
protected:
    std::vector<glm::vec2> calculateInputPins() override;
    std::vector<glm::vec2> calculateOutputPins() override;
public:
    Gate(glm::vec2 cell, MeshRenderer* mesh, std::string text, Sim::Simulation* simulation, std::shared_ptr<Sim::Node> simNode);
    void onMove(glm::vec2 newCell, bool updateSSBO) override;
    void onInputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
};


#endif //BUILDIT_GATE_H
