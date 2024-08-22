//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(intVec2 pos, intVec2 size, NodeRenderer* renderer) : pos(pos), size(size), renderer(renderer) {}

void Node::onMove(intVec2 newPos, bool updateSSBO) {
    this->pos = newPos;
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

bool Nodes::isOccupied(intVec2 cell, intVec2 size, std::unordered_set<std::shared_ptr<Node>> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&cell, &size, &ignored](const std::pair<intVec2, std::shared_ptr<Node>>& entry){
        if (ignored.contains(entry.second)) return false;
        return cell.x-size.x >= entry.first.x && cell.y-size.y >= entry.first.y &&
               cell.x <= entry.first.x + entry.second->size.x && cell.y <= entry.first.y + entry.second->size.y;
    });
}

void Nodes::updatePos(const std::shared_ptr<Node>& node, intVec2 newPos, bool updateSSBO) {
    this->removePins(node);
    this->updatePinPos(node->pos, newPos);
    node->onMove(newPos, updateSSBO);
    this->addPins(node);
}

void Nodes::updatePos(intVec2 oldPos, intVec2 newPos, bool updateSSBO) {
    this->updatePos(this->nodes[oldPos], newPos, updateSSBO);
}

void Nodes::addNode(const std::shared_ptr<Node>& node) {
    this->nodes[node->pos] = node;
    this->addPins(node);
    this->updatePins();
}

void Nodes::removeNode(const std::shared_ptr<Node>& node) {
    this->nodes.erase(node->pos);
    this->removePins(node);
    this->updatePins();
}

void Nodes::removePins(const std::shared_ptr<Node>& node) {
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

void Nodes::addPins(const std::shared_ptr<Node>& node) {
    for (const auto &item: node->inputPins) {
        this->inputPins[item] = node;
    }
    for (const auto &item: node->outputPins) {
        this->outputPins[item] = node;
    }
}

std::shared_ptr<Node> Nodes::getNode(intVec2 cell) {
    auto iterator = std::find_if(this->nodes.begin(), this->nodes.end(), [&cell](const std::pair<intVec2, std::shared_ptr<Node>>& entry){
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
        this->pins.emplace_back(item.first.x*32, item.first.y*32);
    }
    for (const auto &item: this->outputPins) {
        this->pins.emplace_back(item.first.x*32, item.first.y*32);
    }
    this->pinRenderer.updateVertices(&this->pins);
}

void Nodes::updatePinPos(intVec2 oldPos, intVec2 newPos) {
    const auto iter = std::find(this->pins.begin(), this->pins.end(), glm::vec2(oldPos.x*32, oldPos.y*32));
    int index = int(std::distance(this->pins.begin(), iter));
    this->pins[index] = newPos;
    this->pinRenderer.updateVertex(index, glm::vec2(newPos.x*32, newPos.y*32));
}

Nodes::Nodes() {
    this->pinRenderer.init();
}