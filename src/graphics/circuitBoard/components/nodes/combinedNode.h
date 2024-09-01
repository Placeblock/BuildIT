//
// Created by felix on 8/29/24.
//

#ifndef BUILDIT_COMBINEDNODE_H
#define BUILDIT_COMBINEDNODE_H


#include "node.h"

class CombinedNode : public Node {
private:
    std::vector<uintVec2> calculateInputPins() override;
    std::vector<uintVec2> calculateOutputPins() override;
public:
    void move(glm::vec2 newPos, bool updateBuffer) override;

    SimNodeData getInputSimNode(uint8_t inputIndex) override;
    SimNodeData getOutputSimNode(uint8_t outputIndex) override;

    uint32_t getInput() override;
    uint32_t getOutput() override;
    bool getInput(uint8_t index) override;
    bool getOutput(uint8_t index) override;
    bool resetUpdated() override;

    void addToSimulation(Sim::Simulation *sim) override;
    void removeFromSimulation(Sim::Simulation *sim) override;
};


#endif //BUILDIT_COMBINEDNODE_H
