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
#include "graphics/types.h"

class Nodes;

class Node {
protected:
    virtual std::vector<intVec2> calculateInputPins() = 0;
    virtual std::vector<intVec2> calculateOutputPins() = 0;
public:
    Node(intVec2 cell, intVec2 size);
    const intVec2 size;
    intVec2 cell;
    std::vector<intVec2> inputPins;
    std::vector<intVec2> outputPins;
    virtual void onMove(intVec2 newCell, bool updateSSBO);
    virtual void onInputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
};

class Nodes {
private:
    void removePins(const std::shared_ptr<Node>& node);
    void addPins(const std::shared_ptr<Node>& node);
    void updatePins();
    void updatePinPos(intVec2 oldPos, intVec2 newPos);
public:
    Nodes();
    std::unordered_map<intVec2, std::shared_ptr<Node>> nodes;
    std::unordered_map<intVec2, std::shared_ptr<Node>> inputPins;
    std::unordered_map<intVec2, std::shared_ptr<Node>> outputPins;
    std::vector<glm::vec2> pins;
    InstancedVertexRenderer pinRenderer{};
    void updateCell(const std::shared_ptr<Node>& node, intVec2 newCell, bool updateSSBO);
    void updateCell(intVec2 oldCell, intVec2 newCell, bool updateSSBO);
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(const std::shared_ptr<Node>& node);
    bool isOccupied(intVec2 cell, intVec2 size, std::unordered_set<std::shared_ptr<Node>> ignored);
    std::shared_ptr<Node> getNode(intVec2 cell);
};


#endif //BUILDIT_NODE_H
