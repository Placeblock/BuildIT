//
// Created by felix on 29.03.25.
//

#include "Graph.h"

using namespace Sim;

void Graph::connect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Node> &childNode,
                    const unsigned int childIndex) {
    const std::shared_ptr<Pin> sParentPin = parentPin.lock();
    this->outputLinks[sParentPin.get()].insert(childNode);
    childNode.lock()->inputPins[childIndex] = parentPin;
}

void Graph::disconnect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Node> &childNode,
                       const unsigned int childIndex) {
    const std::shared_ptr<Pin> sParentPin = parentPin.lock();
    this->outputLinks[sParentPin.get()].erase(childNode);
    childNode.lock()->inputPins[childIndex] = std::weak_ptr<Pin>();
}
