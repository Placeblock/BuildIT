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
    Node(glm::vec2 cell, intVec2 size, Sim::Simulation* simulation, const std::shared_ptr<Sim::Node>& simNode, NodeRenderer* renderer);
    Sim::Simulation* simulation;
    const std::shared_ptr<Sim::Node> simNode;
    const intVec2 size;
    glm::vec2 cell;
    std::vector<uintVec2> inputPins;
    std::vector<uintVec2> outputPins;
    uint getInputPinIndex(glm::vec2 absInputPin);
    uint getOutputPinIndex(glm::vec2 absOutputPin);
    NodeRenderer* renderer;
    virtual void onMove(glm::vec2 newPos, bool updateSSBO);

    [[nodiscard]] bool isInside(glm::vec2 checkCell) const;

    virtual ~Node() {
        std::cout << "Deconstructing Node\n";
    };
};

struct Pin {
    Node* node;
    uint8_t index;
};

class Nodes {
private:
    void removePins(Node* node);
    void addPins(Node* node);
    void updatePins();
    void updateNodePins(Node* node, glm::vec2 newCell);
    void updatePinCell(glm::vec2 oldCell, glm::vec2 newCell);
public:
    Nodes();
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> nodes;
    std::unordered_map<glm::vec2, Node*> inputPins; // All input Pins formatted as cells
    std::unordered_map<glm::vec2, Node*> outputPins; // All output Pins formatted as cells
    std::vector<glm::vec2> pins; // All Pins of all Nodes but formatted as position, not cell! (*32)
    InstancedVertexRenderer pinRenderer{};
    void updatePos(Node* node, glm::vec2 newPos, bool updateSSBO);
    void updatePos(glm::vec2 oldPos, glm::vec2 newPos, bool updateSSBO);
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(Node* node);
    bool isOccupied(glm::vec2 cell, std::unordered_set<Node*> ignored);
    std::shared_ptr<Node> getNode(glm::vec2 cell);
};


#endif //BUILDIT_NODE_H
