//
// Created by felix on 8/31/24.
//

#include "nodesFeature.h"

NodesFeature::NodesFeature(Programs *programs, Subject<ComponentAddEvent> *addSubject, Subject<ComponentRemoveEvent> *removeSubject)
    : Renderable(programs) {
    addSubject->subscribe(&this->nodePins);
    removeSubject->subscribe(&this->nodePins);
    addSubject->subscribe(this);
    removeSubject->subscribe(this);
}

void NodesFeature::render() {
    nodePins.render(programs->pinProgram);
}

NodePinHandler *NodesFeature::getNodePinHandler() {
    return &this->nodePins;
}

void NodesFeature::notify(const ComponentAddEvent &event) {
    if (Node *node = dynamic_cast<Node*>(event.component)) {
        this->nodes.insert(node);
    }
}

void NodesFeature::notify(const ComponentRemoveEvent &event) {
    if (Node *node = dynamic_cast<Node*>(event.component)) {
        this->nodes.erase(node);
    }
}

std::unordered_set<Node *>* NodesFeature::getNodes() {
    return &this->nodes;
}
