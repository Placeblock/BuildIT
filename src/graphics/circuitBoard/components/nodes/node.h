//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <unordered_set>
#include <iostream>
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"

#include "graphics/renderer/instancedVertexRenderer.h"
#include "graphics/types.h"
#include "simulation/simulation.h"

class Node;

#include "graphics/circuitBoard/renderer/node/nodeRenderer.h"

class Node {
protected:
    virtual std::vector<uintVec2> calculateInputPins() = 0;
    virtual std::vector<uintVec2> calculateOutputPins() = 0;
public:
    Node(glm::vec2 cell, intVec2 size, const std::shared_ptr<Sim::Node>& simNode, NodeRenderer* renderer);

    const std::shared_ptr<Sim::Node> simNode;
    const intVec2 size;
    glm::vec2 cell;
    std::vector<uintVec2> inputPins;
    std::vector<uintVec2> outputPins;
    uint8_t getInputPinIndex(glm::vec2 absInputPin);
    uint8_t getOutputPinIndex(glm::vec2 absOutputPin);
    NodeRenderer* renderer;
    virtual void onMove(glm::vec2 newPos, bool updateBuffer);

    [[nodiscard]] bool isInside(glm::vec2 checkCell) const;

    virtual ~Node() {
        std::cout << "Deconstructing Node\n";
    };
};

class Pin {
public:
    Node* node;
    uint8_t index;

    bool operator==(const Pin &other) const {
        return (node == other.node && index == other.index);
    }
};

template <>
struct std::hash<Pin> {
    std::size_t operator()(const Pin& p) const {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<Node*>()(p.node) ^ (hash<int>()(p.index) << 1)) >> 1);
    }
};


#endif //BUILDIT_NODE_H
