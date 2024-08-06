//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(Nodes* nodes, glm::vec2 cell, glm::vec2 size, MeshRenderer* mesh) : nodes(nodes), cell(cell), size(size), mesh(mesh) {
    mesh->addInstance(cell*glm::vec2(32, 32));
}

void Node::onMove(glm::vec2 newCell, bool updateSSBO) {
    this->mesh->updateInstance(this->cell, newCell, updateSSBO);
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}


bool Nodes::isOccupied(glm::vec2 cell, glm::vec2 size, std::unordered_set<std::shared_ptr<Node>> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &size, &ignored](const std::pair<glm::vec2, std::shared_ptr<Node>>& entry){
        if (ignored.contains(entry.second)) return false;
        return cell.x-size.x >= entry.first.x && cell.y-size.y >= entry.first.y &&
               cell.x <= entry.first.x + entry.second->size.x && cell.y <= entry.first.y + entry.second->size.y;
    });
}

void Nodes::updateCell(std::shared_ptr<Node> node, glm::vec2 newCell, bool updateSSBO) {
    this->removePins(node);
    this->updatePinPos(node->cell, newCell);
    node->onMove(newCell, updateSSBO);
    this->addPins(node);
}

void Nodes::updateCell(glm::vec2 oldCell, glm::vec2 newCell, bool updateSSBO) {
    this->updateCell(this->nodes[oldCell], newCell, updateSSBO);
}

void Nodes::addNode(std::shared_ptr<Node> node) {
    this->nodes[node->cell] = node;
    this->addPins(node);
    this->updatePins();
}

void Nodes::removeNode(std::shared_ptr<Node> node) {
    this->nodes.erase(node->cell);
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
    auto iterator = std::find_if(this->nodes.begin(), this->nodes.end(), [&cell](const std::pair<glm::vec2, std::shared_ptr<Node>>& entry){
       return entry.first.x <= cell.x && entry.first.y <= cell.y &&
        entry.first.x + entry.second->size.x >= cell.x && entry.first.y + entry.second->size.y >= cell.y;
    });
    if (iterator == this->nodes.end()) return nullptr;
    return iterator->second;
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