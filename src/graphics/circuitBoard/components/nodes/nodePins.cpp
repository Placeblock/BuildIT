//
// Created by felix on 9/1/24.
//

#include "nodePins.h"

void NodePins::removePins(Node* node) {
    for (const auto &item: node->inputPins) {
        glm::vec2 absCell = node->getPos() / 32.0f + glm::vec2(item);
        if (this->inputPinCells[absCell] == node) {
            this->inputPinCells.erase(absCell);
        }
    }
    for (const auto &item: node->outputPins) {
        glm::vec2 absCell = node->getPos() / 32.0f + glm::vec2(item);
        if (this->outputPinCells[absCell] == node) {
            this->outputPinCells.erase(absCell);
        }
    }
}

void NodePins::addPins(Node* node) {
    for (const auto &item: node->inputPins) {
        glm::vec2 abs = node->getPos() / 32.0f + glm::vec2(item);
        assert(!this->inputPinCells.contains(abs) && "Tried to overwrite node pin");
        this->inputPinCells[abs] = node;
    }
    for (const auto &item: node->outputPins) {
        glm::vec2 abs = node->getPos() / 32.0f + glm::vec2(item);
        assert(!this->inputPinCells.contains(abs) && "Tried to overwrite node pin");
        this->outputPinCells[abs] = node;
    }
}

void NodePins::updatePins() {
    this->pins.clear();
    this->pins.reserve(this->inputPinCells.size() + this->outputPinCells.size());
    for (const auto &item: this->inputPinCells) {
        this->pins.push_back(item.first * 32.0f);
    }
    for (const auto &item: this->outputPinCells) {
        this->pins.push_back(item.first * 32.0f);
    }
    this->pinRenderer.updateVertices(&this->pins);
}

void NodePins::updatePinPos(glm::vec2 oldPos, glm::vec2 newPos) {
    const auto iter = std::find(this->pins.begin(), this->pins.end(), oldPos * 32.0f);
    int index = int(std::distance(this->pins.begin(), iter));
    this->pins[index] = newPos * 32.0f;
    if (this->inputPinCells.contains(oldPos)) {
        this->inputPinCells[newPos] = this->inputPinCells[oldPos];
        this->inputPinCells.erase(oldPos);
    }
    if (this->outputPinCells.contains(oldPos)) {
        this->outputPinCells[newPos] = this->outputPinCells[oldPos];
        this->outputPinCells.erase(oldPos);
    }
    this->pinRenderer.updateVertex(index, newPos * 32.0f);
}

void NodePins::updateNodePins(Node *node, glm::vec2 newPos) {
    for (const auto &item: node->inputPins) {
        this->updatePinPos(node->getPos() / 32.0f + glm::vec2(item), newPos / 32.0f + glm::vec2(item));
    }
    for (const auto &item: node->outputPins) {
        this->updatePinPos(node->getPos() / 32.0f + glm::vec2(item), newPos / 32.0f + glm::vec2(item));
    }
}

void NodePins::notify(const MoveEvent &event) {
    if (!event.before) return;
    if (Node *node = dynamic_cast<Node*>(event.movable)) {
        this->updateNodePins(node, event.newPos);
    }

}

void NodePins::notify(const RotateEvent &event) {
    if (!event.before) return;
    if (Node *node = dynamic_cast<Node*>(event.rotatable)) {
        this->updateNodePins(node, node->getPos());
    }
}

void NodePins::notify(const ComponentAddEvent &data) {
    if (Node *node = dynamic_cast<Node*>(data.component)) {
        this->addPins(node);
        this->updatePins();
        node->Movable::subscribe(this);
        node->Rotatable::subscribe(this);
    }
}

void NodePins::notify(const ComponentRemoveEvent &data) {
    if (Node *node = dynamic_cast<Node*>(data.component)) {
        this->removePins(node);
        this->updatePins();
        node->Movable::unsubscribe(this);
        node->Rotatable::unsubscribe(this);
    }
}

bool NodePins::isInputPin(glm::vec2 pos) {
    return this->inputPinCells.contains(pos / 32.0f);
}

bool NodePins::isOutputPin(glm::vec2 pos) {
    return this->outputPinCells.contains(pos / 32.0f);
}

bool NodePins::isPin(glm::vec2 pos) {
    return this->isInputPin(pos) || this->isOutputPin(pos);
}

Node* NodePins::getNode(glm::vec2 pos) {
    if (this->inputPinCells.contains(pos / 32.0f)) return this->inputPinCells[pos / 32.0f];
    if (this->outputPinCells.contains(pos / 32.0f)) return this->outputPinCells[pos / 32.0f];
    return nullptr;
}

void NodePins::render(Program *program) {
    this->pinRenderer.render(program);
}

NodePins::NodePins() {
    this->pinRenderer.init();
}
