//
// Created by felix on 29.03.25.
//

#include "Graph.h"

using namespace Sim;

void Graph::connect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Pin> &childPin) {
    this->outputLinks[parentPin].insert(childPin);
    this->inputLinks[childPin] = parentPin;
}

void Graph::disconnect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Pin> &childPin) {
    this->outputLinks[parentPin].erase(childPin);
    this->inputLinks.erase(childPin);
}
