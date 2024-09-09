//
// Created by felix on 9/1/24.
//

#include "nodePins.h"

void NodePins::removePins(Node* node) {
    for (const auto &item: node->inputPins) {
        glm::vec2 absCell = node->getPos() + glm::vec2(item);
        if (this->inputPins[absCell] == node) {
            this->inputPins.erase(absCell);
        }
    }
    for (const auto &item: node->outputPins) {
        glm::vec2 absCell = node->getPos() + glm::vec2(item);
        if (this->outputPins[absCell] == node) {
            this->outputPins.erase(absCell);
        }
    }
}

void NodePins::addPins(Node* node) {
    for (const auto &item: node->inputPins) {
        glm::vec2 abs = node->getPos() + glm::vec2(item);
        assert(!this->inputPins.contains(abs) && "Tried to overwrite node pin");
        this->inputPins[abs] = node;
    }
    for (const auto &item: node->outputPins) {
        glm::vec2 abs = node->getPos() + glm::vec2(item);
        assert(!this->inputPins.contains(abs) && "Tried to overwrite node pin");
        this->outputPins[abs] = node;
    }
}

void NodePins::updatePins() {
    this->pins.clear();
    this->pins.reserve(this->inputPins.size() + this->outputPins.size());
    for (const auto &item: this->inputPins) {
        this->pins.push_back(item.first);
    }
    for (const auto &item: this->outputPins) {
        this->pins.push_back(item.first);
    }
    this->pinRenderer.updateVertices(&this->pins);
}

void NodePins::updatePinPos(glm::vec2 oldPos, glm::vec2 newPos) {
    const auto iter = std::find(this->pins.begin(), this->pins.end(), glm::vec2(oldPos.x, oldPos.y));
    int index = int(std::distance(this->pins.begin(), iter));
    this->pins[index] = newPos;
    if (this->inputPins.contains(oldPos)) {
        this->inputPins[newPos] = this->inputPins[oldPos];
        this->inputPins.erase(oldPos);
    }
    if (this->outputPins.contains(oldPos)) {
        this->outputPins[newPos] = this->outputPins[oldPos];
        this->outputPins.erase(oldPos);
    }
    this->pinRenderer.updateVertex(index, newPos);
}

void NodePins::updateNodePins(Node *node, glm::vec2 newPos) {
    for (const auto &item: node->inputPins) {
        this->updatePinPos(node->getPos() + glm::vec2(item), newPos + glm::vec2(item));
    }
    for (const auto &item: node->outputPins) {
        this->updatePinPos(node->getPos() + glm::vec2(item), newPos + glm::vec2(item));
    }
}

void NodePins::notify(Subject<MoveEvent> *subject, const MoveEvent &event) {
    if (Node *node = dynamic_cast<Node*>(subject)) {
        this->updateNodePins(node, event.newPos);
    }

}

void NodePins::notify(Subject<RotateEvent> *subject, const RotateEvent &event) {
    if (Node *node = dynamic_cast<Node*>(subject)) {
        this->updateNodePins(node, node->getPos());
    }
}

void NodePins::notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent &data) {
    if (Node *node = dynamic_cast<Node*>(data.component)) {
        this->addPins(node);
        this->updatePins();
        node->Movable::subscribe(this);
        node->Rotatable::subscribe(this);
    }
}

void NodePins::notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent &data) {
    if (Node *node = dynamic_cast<Node*>(data.component)) {
        this->removePins(node);
        this->updatePins();
        node->Movable::unsubscribe(this);
        node->Rotatable::unsubscribe(this);
    }
}

bool NodePins::isInputPin(glm::vec2 pos) {
    return this->inputPins.contains(pos);
}

bool NodePins::isOutputPin(glm::vec2 pos) {
    return this->outputPins.contains(pos);
}

bool NodePins::isPin(glm::vec2 pos) {
    return this->isInputPin(pos) || this->isOutputPin(pos);
}

Node* NodePins::getNode(glm::vec2 pos) {
    if (this->inputPins.contains(pos)) return this->inputPins[pos];
    if (this->outputPins.contains(pos)) return this->outputPins[pos];
    return nullptr;
}

void NodePins::render(Program *program) {
    this->pinRenderer.render(program);
}
