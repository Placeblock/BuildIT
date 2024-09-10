//
// Created by felix on 8/31/24.
//

#include "nodesFeature.h"

NodesFeature::NodesFeature(Programs *programs)
    : Renderable(programs) {

}

void NodesFeature::render() {
    nodePins.render(programs->pinProgram);
}

NodePinHandler *NodesFeature::getNodePinHandler() {
    return &this->nodePins;
}
