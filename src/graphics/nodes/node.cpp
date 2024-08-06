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
    });
}

void Nodes::updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO) {
    this->removeCells(node);
    node->updateCell(newCell, updateSSBO);
    this->addCells(node);
}

void Nodes::addNode(std::shared_ptr<Node> node) {
    this->nodes.insert(node);
    this->addCells(node);
}

void Nodes::removeNode(std::shared_ptr<Node> node) {
    this->nodes.erase(node);
    this->removeCells(node);
}

void Nodes::removeCells(std::shared_ptr<Node> node) {
    for (const auto &item: node->inputCells) {
        this->inputCells.erase(item);
    }
    for (const auto &item: node->outputCells) {
        this->outputCells.erase(item);
    }
}

void Nodes::addCells(std::shared_ptr<Node> node) {
    for (const auto &item: node->inputCells) {
        this->inputCells[item] = node;
    }
    for (const auto &item: node->outputCells) {
        this->outputCells[item] = node;
    }
}

std::shared_ptr<Node> Nodes::getNode(glm::vec2 cell) {
    auto iterator = std::find_if(this->nodes.begin(), this->nodes.end(), [&cell](std::shared_ptr<Node> node){
       return node->cell.x <= cell.x && node->cell.y <= cell.y &&
        node->cell.x + node->size.x >= cell.x && node->cell.y + node->size.y >= cell.y;
    });
    if (iterator == this->nodes.end()) return nullptr;
    return *iterator;
}
