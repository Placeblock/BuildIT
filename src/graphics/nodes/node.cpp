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

bool Nodes::isOccupied(glm::vec2 cell, glm::vec2 size, std::unordered_set<std::shared_ptr<Node>> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &size, &ignored](const std::shared_ptr<Node>& node){
        if (ignored.contains(node)) return false;
        return cell.x-size.x >= node->cell.x && cell.y-size.y >= node->cell.y &&
               cell.x <= node->cell.x + node->size.x && cell.y <= node->cell.y + node->size.y;
    })
}

void Nodes::updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO) {
    for (const auto &item: node->inputCells) {
        this->inputCells.erase(item);
    }
    for (const auto &item: node->outputCells) {
        this->outputCells.erase(item);
    }
    node->updateCell(newCell, updateSSBO);
    for (const auto &item: node->inputCells) {
        this->inputCells[item] = node;
    }
    for (const auto &item: node->outputCells) {
        this->outputCells[item] = node;
    }
}
