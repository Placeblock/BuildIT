//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 cell, glm::vec2 size, Mesh* mesh) : cell(cell), size(size), mesh(mesh) {
    mesh->addInstance(cell*glm::vec2(32, 32));
}

void Node::updateCell(glm::vec2 newCell, bool updateSSBO) {
    this->mesh->updateInstance(this->cell, newCell, updateSSBO);
    this->cell = newCell;
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

bool Nodes::isOccupied(glm::vec2 cell, glm::vec2 size, std::unordered_set<std::shared_ptr<Node>> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &size, &ignored](const std::shared_ptr<Node>& node){
        if (ignored.contains(node)) return false;
        return cell.x-size.x >= node->cell.x && cell.y-size.y >= node->cell.y &&
               cell.x <= node->cell.x + node->size.x && cell.y <= node->cell.y + node->size.y;
    });
}

void Nodes::updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO) {
    this->removePins(node);
    this->updatePinPos(node->cell, newCell);
    node->updateCell(newCell, updateSSBO);
    this->addPins(node);
}

void Nodes::addNode(std::shared_ptr<Node> node) {
    this->nodes.insert(node);
    this->addPins(node);
    this->updatePins();
}

void Nodes::removeNode(std::shared_ptr<Node> node) {
    this->nodes.erase(node);
    this->removePins(node);
    this->updatePins();
}

void Nodes::removePins(std::shared_ptr<Node> node) {
    for (const auto &item: node->inputPins) {
        if (this->inputPins[item] == node) {
            this->inputPins.erase(item);
        }
    }
    for (const auto &item: node->outputPins) {
        if (this->outputPins[item] == node) {
            this->outputPins.erase(item);
        }
    }
}

void Nodes::addPins(std::shared_ptr<Node> node) {
    for (const auto &item: node->inputPins) {
        this->inputPins[item] = node;
    }
    for (const auto &item: node->outputPins) {
        this->outputPins[item] = node;
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

void Nodes::updatePins() {
    this->pins.clear();
    this->pins.reserve(this->inputPins.size() + this->outputPins.size());
    for (const auto &item: this->inputPins) {
        this->pins.push_back(item.first*glm::vec2(32, 32));
    }
    for (const auto &item: this->outputPins) {
        this->pins.push_back(item.first*glm::vec2(32, 32));
    }
    this->pinRenderer.updateVertices(&this->pins);
}

void Nodes::updatePinPos(glm::vec2 oldPos, glm::vec2 newPos) {
    const auto iter = std::find(this->pins.begin(), this->pins.end(), oldPos*glm::vec2(32, 32));
    int index = std::distance(this->pins.begin(), iter);
    this->pins[index] = newPos;
    this->pinRenderer.updateVertex(index, newPos*glm::vec2(32, 32));
}

Nodes::Nodes() {
    this->pinRenderer.init();
}
