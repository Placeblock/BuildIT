//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 cell, intVec2 size, Sim::Simulation* simulation, const std::shared_ptr<Sim::Node>& simNode, NodeRenderer* renderer)
    : cell(cell), simulation(simulation), size(size), simNode(simNode), renderer(renderer) {}

void Node::onMove(glm::vec2 newPos, bool updateSSBO) {
    this->cell = newPos;
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

bool Node::isInside(glm::vec2 checkCell) const {
    return checkCell.x >= this->cell.x && checkCell.y >= this->cell.y
           && checkCell.x <= this->cell.x + float(this->size.x) && checkCell.y <= this->cell.y + float(this->size.y);
}

void Node::onUpdate() {

}

uint Node::getInputPinIndex(glm::vec2 absInputPin) {
    const auto iter = std::find(this->inputPins.begin(), this->inputPins.end(), uintVec2(absInputPin - glm::vec2(this->cell)));
    return std::distance(this->inputPins.begin(), iter);
}

uint Node::getOutputPinIndex(glm::vec2 absOutputPin) {
    const auto iter = std::find(this->outputPins.begin(), this->outputPins.end(), uintVec2(absOutputPin - glm::vec2(this->cell)));
    return std::distance(this->outputPins.begin(), iter);
}

bool Nodes::isOccupied(glm::vec2 cell, std::unordered_set<Node*> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &ignored](const std::pair<glm::vec2, std::shared_ptr<Node>>& entry){
        if (ignored.contains(entry.second.get())) return false;
        return entry.second->isInside(cell);
    });
}

void Nodes::updatePos(Node* node, glm::vec2 newPos, bool updateSSBO) {
    this->removePins(node);
    this->updateNodePins(node, newPos);
    node->onMove(newPos, updateSSBO);
    this->addPins(node);
}

void Nodes::updatePos(glm::vec2 oldPos, glm::vec2 newPos, bool updateSSBO) {
    this->updatePos(this->nodes[oldPos].get(), newPos, updateSSBO);
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