//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <unordered_set>
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "graphics/circuitBoard/elements/wires/wires.h"
#include "graphics/renderer/instancedVertexRenderer.h"
#include "graphics/types.h"

class Node;

#include "graphics/circuitBoard/renderer/node/nodeRenderer.h"

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
    NodeRenderer* renderer;
    virtual void onMove(glm::vec2 newPos, bool updateSSBO);
    virtual void onInputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;

    [[nodiscard]] bool isInside(glm::vec2 checkCell) const;

    virtual ~Node() {
        std::cout << "Deconstructing Node\n";
    };
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
    std::unordered_map<glm::vec2, Node*> inputPins;
    std::unordered_map<glm::vec2, Node*> outputPins;
    std::vector<glm::vec2> pins;
    InstancedVertexRenderer pinRenderer{};
    void updatePos(Node* node, glm::vec2 newPos, bool updateSSBO);
    void updatePos(glm::vec2 oldPos, glm::vec2 newPos, bool updateSSBO);
    void addNode(std::shared_ptr<Node> node);
    void removeNode(Node* node);
    bool isOccupied(glm::vec2 cell, std::unordered_set<Node*> ignored);
    std::shared_ptr<Node> getNode(glm::vec2 cell);
};


#endif //BUILDIT_NODE_H
