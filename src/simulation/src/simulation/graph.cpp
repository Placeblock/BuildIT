//
// Created by felix on 29.03.25.
//

#include "simulation/graph.hpp"

using namespace Sim;

template <typename T>
void Graph::connect(Pin<T> &parentPin, Node &childNode, PinSink<T> &childPinSink) {
    parentPin->nodes.insert(childNode);
    childPinSink.pin = parentPin;
}

template <typename T>
void Graph::disconnect(Pin<T> &parentPin, Node &childNode, PinSink<T> &childPinSink) {
    parentPin->nodes.erase(childNode);
    childPinSink.pin = nullptr;
}
