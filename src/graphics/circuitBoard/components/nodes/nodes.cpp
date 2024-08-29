//
// Created by felix on 8/28/24.
//

#include "nodes.h"


bool Nodes::isOccupied(glm::vec2 cell, std::unordered_set<Node*> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &ignored](const std::pair<glm::vec2, std::shared_ptr<Node>>& entry){
        if (ignored.contains(entry.second.get())) return false;
        return entry.second->isInside(cell);
    });
}

void Nodes::moveNode(Node* node, glm::vec2 newPos, bool updateBuffer) {
    this->removePins(node);
    this->updateNodePins(node, newPos);
    node->onMove(newPos, updateBuffer);
    this->addPins(node);
}

void Nodes::addNode(const std::shared_ptr<Node>& node) {
    this->nodes[node->cell] = node;
    this->addPins(node.get());
    this->updatePins();
}

void Nodes::removeNode(Node* node) {
    this->nodes.erase(node->cell);
    this->removePins(node);
    this->updatePins();
}

void Nodes::removePins(Node* node) {
    for (const auto &item: node->inputPins) {
        glm::vec2 absCell = node->cell + glm::vec2(item);
        if (this->inputPins[absCell] == node) {
            this->inputPins.erase(absCell);
        }
    }
    for (const auto &item: node->outputPins) {
        glm::vec2 absCell = node->cell + glm::vec2(item);
        if (this->outputPins[absCell] == node) {
            this->outputPins.erase(absCell);
        }
    }
}

void Nodes::addPins(Node* node) {
    for (const auto &item: node->inputPins) {
        this->inputPins[node->cell + glm::vec2(item)] = node;
    }
    for (const auto &item: node->outputPins) {
        this->outputPins[node->cell + glm::vec2(item)] = node;
    }
}

std::shared_ptr<Node> Nodes::getNode(glm::vec2 cell) {
    auto iterator = std::find_if(this->nodes.begin(), this->nodes.end(), [&cell](const std::pair<glm::vec2, std::shared_ptr<Node>>& entry){
        return entry.second->isInside(cell);
    });
    if (iterator == this->nodes.end()) return nullptr;
    return iterator->second;
}

void Nodes::updatePins() {
    this->pins.clear();
    this->pins.reserve(this->inputPins.size() + this->outputPins.size());
    for (const auto &item: this->inputPins) {
        this->pins.emplace_back(item.first.x*32, item.first.y*32);
    }
    for (const auto &item: this->outputPins) {
        this->pins.emplace_back(item.first.x*32, item.first.y*32);
    }
    this->pinRenderer.updateVertices(&this->pins);
}

void Nodes::updatePinCell(glm::vec2 oldCell, glm::vec2 newCell) {
    const auto iter = std::find(this->pins.begin(), this->pins.end(), glm::vec2(oldCell.x * 32, oldCell.y * 32));
    int index = int(std::distance(this->pins.begin(), iter));
    this->pins[index] = newCell * 32.0f;
    this->pinRenderer.updateVertex(index, newCell * 32.0f);
}

void Nodes::updateNodePins(Node *node, glm::vec2 newCell) {
    for (const auto &item: node->inputPins) {
        this->updatePinCell(node->cell + glm::vec2(item), newCell + glm::vec2(item));
    }
    for (const auto &item: node->outputPins) {
        this->updatePinCell(node->cell + glm::vec2(item), newCell + glm::vec2(item));
    }
}


Nodes::Nodes() {
    this->pinRenderer.init();
}