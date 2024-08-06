//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 cell, glm::vec2 size, Mesh* mesh) : cell(cell), size(size), mesh(mesh) {
}

void Node::updateCell(glm::vec2 newCell, bool updateSSBO) {
    this->mesh->updateInstance(this->cell, newCell, updateSSBO);
    this->cell = newCell;
    this->inputCells = this->calculateInputCells();
    this->outputCells = this->calculateOutputCells();
}
