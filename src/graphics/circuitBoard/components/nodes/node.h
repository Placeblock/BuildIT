//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H

#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_set>
#include <iostream>
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"

#include "graphics/renderer/instancedVertexRenderer.h"
#include "graphics/types.h"
#include "simulation/simulation.h"

class Node;

#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderer.h"

struct SimNodeData {
    Sim::Node* node;
    uint8_t index;
};

class Node {
protected:
    virtual std::vector<uintVec2> calculateInputPins() = 0;
    virtual std::vector<uintVec2> calculateOutputPins() = 0;
public:
    Node(glm::vec2 cell, intVec2 size, NodeRenderer* renderer);

    const intVec2 size;
    glm::vec2 cell;
    std::vector<uintVec2> inputPins;
    std::vector<uintVec2> outputPins;
    uint8_t getInputPinIndex(glm::vec2 absInputPin);
    uint8_t getOutputPinIndex(glm::vec2 absOutputPin);
    NodeRenderer* renderer;
    virtual void onMove(glm::vec2 newPos, bool updateBuffer);

    virtual SimNodeData getInputSimNode(uint8_t inputIndex) = 0;
    virtual SimNodeData getOutputSimNode(uint8_t outputIndex) = 0;

    virtual uint32_t getInput() = 0;
    virtual uint32_t getOutput() = 0;
    virtual bool getInput(uint8_t index) = 0;
    virtual bool getOutput(uint8_t index) = 0;
    virtual bool resetUpdated() = 0;

    virtual void addToSimulation(Sim::Simulation *sim) = 0;
    virtual void removeFromSimulation(Sim::Simulation *sim) = 0;

    [[nodiscard]] bool isInside(glm::vec2 checkCell) const;

    virtual ~Node() {
        std::cout << "Deconstructing Node\n";
    };
};

#endif //BUILDIT_NODE_H
