//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <unordered_set>
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "graphics/elements/wires/wires.h"
#include "graphics/renderer/meshRenderer.h"
#include "graphics/renderer/instancedVertexRenderer.h"

class Nodes;

class Node {
protected:
    Nodes* nodes;
    MeshRenderer* mesh;
    virtual std::vector<glm::vec2> calculateInputPins() = 0;
    virtual std::vector<glm::vec2> calculateOutputPins() = 0;
public:
    Node(Nodes* nodes, glm::vec2 cell, glm::vec2 size, MeshRenderer* mesh);
    const glm::vec2 size;
    glm::vec2 cell;
    std::vector<glm::vec2> inputPins;
    std::vector<glm::vec2> outputPins;
    void onMove(glm::vec2 newCell, bool updateSSBO);
    virtual void onInputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
};

class Nodes {
private:
    void removePins(std::shared_ptr<Node> node);
    void addPins(std::shared_ptr<Node> node);
    void updatePins();
    void updatePinPos(glm::vec2 oldPos, glm::vec2 newPos);
public:
    Nodes();
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> nodes;
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> inputPins;
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> outputPins;
    std::vector<glm::vec2> pins;
    InstancedVertexRenderer pinRenderer{};
    void updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO);
    void updateCell(glm::vec2 oldCell, glm::vec2 newCell, bool updateSSBO);
    void addNode(std::shared_ptr<Node> node);
    void removeNode(std::shared_ptr<Node> node);
    bool isOccupied(glm::vec2 cell, glm::vec2 size, std::unordered_set<std::shared_ptr<Node>> ignored);
    std::shared_ptr<Node> getNode(glm::vec2 cell);
};


#endif //BUILDIT_NODE_H
