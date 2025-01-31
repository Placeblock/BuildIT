//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H

#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_set>
#include <iostream>
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/components/abstraction/rotatable.h"
#include "graphics/circuitBoard/selection/selectable.h"
#include "graphics/circuitBoard/components/abstraction/aABBInteractable.h"
#include "graphics/types.h"
#include "simulation/simulation.h"

class Network;

struct SimNodeData {
    Sim::Node* node;
    uint8_t index;
};

class Node : public Movable, public Rotatable, public Selectable, public AABBInteractable {
private:
    glm::vec2 pos;
protected:
    virtual std::vector<uintVec2> calculateInputPins() = 0;
    virtual std::vector<uintVec2> calculateOutputPins() = 0;
public:
    Node(glm::vec2 pos, intVec2 cellSize);
    Node(Node& other);

    const intVec2 cellSize; // Size in Cells
    std::vector<uintVec2> inputPins;
    std::vector<uintVec2> outputPins;
    std::vector<Network*> outputNetworks;
    uint8_t getInputPinIndex(glm::vec2 absInputPin);
    uint8_t getOutputPinIndex(glm::vec2 absOutputPin);
    void move(glm::vec2 newPos) override;

    virtual SimNodeData getInputSimNode(uint8_t inputIndex) = 0;
    virtual SimNodeData getOutputSimNode(uint8_t outputIndex) = 0;

    virtual uint32_t getInput() = 0;
    virtual uint32_t getOutput() = 0;
    virtual bool getInput(uint8_t index) = 0;
    virtual bool getOutput(uint8_t index) = 0;
    virtual bool resetUpdated() = 0;

    virtual void addToSimulation(Sim::Simulation *sim) = 0;
    virtual void removeFromSimulation(Sim::Simulation *sim) = 0;

    void onMove(glm::vec2 delta) override;
    [[nodiscard]] glm::vec2 getPos() const;

    ~Node() override {
    };
};

#endif //BUILDIT_NODE_H
