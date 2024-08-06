//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <unordered_set>
#include "glm/glm.hpp"
#include "graphics/wires/wires.h"

class Vertex;

class Node {
public:
    glm::vec2 cell;
    const glm::vec2 size;
    std::vector<glm::vec2> inputCells;
    std::vector<glm::vec2> outputCells;
    virtual std::unordered_set<glm::vec2> calculateInputCells() = 0;
    virtual std::unordered_set<glm::vec2> calculateOutputCells() = 0;
    virtual void onInputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) = 0;
    virtual void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) = 0;
};


#endif //BUILDIT_NODE_H
