//
// Created by felix on 8/31/24.
//

#include "nodesFeature.h"

NodesFeature::NodesFeature(Programs *programs, Subject<ComponentAddEvent> *addSubject, Subject<ComponentRemoveEvent> *removeSubject)
    : Renderable(programs) {
    addSubject->subscribe(&this->nodePins);
    removeSubject->subscribe(&this->nodePins);
}

void NodesFeature::render() {
    nodePins.render(programs->pinProgram);
}

NodePinHandler *NodesFeature::getNodePinHandler() {
    return &this->nodePins;
}
