//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_GATE_H
#define BUILDIT_GATE_H

#include <string>
#include <set>
#include <memory>
#include "node.h"
#include "simulation/node.h"
#include "simulation/simulation.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
protected:
    std::vector<uintVec2> calculateInputPins() override;
    std::vector<uintVec2> calculateOutputPins() override;
public:
    const std::string text;
    const std::shared_ptr<Sim::Node> simNode;

    Gate(glm::vec2 cell, std::string text, const std::shared_ptr<Sim::Node>& simNode);
    Gate(Gate& other);

    SimNodeData getInputSimNode(uint8_t inputIndex) override;
    SimNodeData getOutputSimNode(uint8_t outputIndex) override;

    uint32_t getOutput() override;
    uint32_t getInput() override;
    bool getInput(uint8_t index) override;
    bool getOutput(uint8_t index) override;
    bool resetUpdated() override;

    void addToSimulation(Sim::Simulation *sim) override;
    void removeFromSimulation(Sim::Simulation *sim) override;

    ~Gate() override = default;

    static intVec2 calcSize(const std::shared_ptr<Sim::Node>& simNode);
};


#endif //BUILDIT_GATE_H
