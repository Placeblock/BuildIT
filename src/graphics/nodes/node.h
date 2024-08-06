//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <unordered_set>
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "graphics/wires/wires.h"
#include "mesh.h"

class Node {
protected:
    Mesh* mesh;
    virtual std::vector<glm::vec2> calculateInputCells() = 0;
    virtual std::vector<glm::vec2> calculateOutputCells() = 0;
public:
    Node(glm::vec2 cell, glm::vec2 size, Mesh* mesh);
    const glm::vec2 size;
    glm::vec2 cell;
    std::vector<glm::vec2> inputCells;
    std::vector<glm::vec2> outputCells;
    void updateCell(glm::vec2 newCell, bool updateSSBO);
    virtual void onInputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
};

class Nodes {
public:
    std::unordered_set<std::shared_ptr<Node>> nodes;
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> inputCells;
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> outputCells;
    void updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO);
    bool isOccupied(glm::vec2 cell, glm::vec2 size, std::unordered_set<std::shared_ptr<Node>> ignored);
};


#endif //BUILDIT_NODE_H
