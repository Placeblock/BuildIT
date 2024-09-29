//
// Created by felix on 9/29/24.
//

#include "network.h"
#include "graphics/util.h"

std::shared_ptr<Network> Networkable::getNetwork() const {
    return this->network;
}

void Networkable::setNetwork(std::shared_ptr<Network> newNetwork) {
    if (newNetwork == this->network) return;
    this->notify({this, newNetwork.get(), true});
    this->network = std::move(newNetwork);
    this->notify({this, this->network.get()});
}

Networkable::Networkable(std::shared_ptr<Network> network) : network(std::move(network)) {

}

void Network::removeWire(Wire* wire) {
    this->wires.remove(wire);
}

void Network::removeJoint(Joint* joint) {
    this->joints.remove(joint);
}

void Network::connect(Sim::Simulation* sim, const Pin& parent, const Pin& child) {
    const auto parentRef = Sim::Reference(parent.getOutputSimNode(), child.getInputSimNode(), parent.index);
    const auto childRef = Sim::Reference(child.getInputSimNode(), parent.getOutputSimNode(), child.index);
    sim->connect(parentRef, childRef, true);
}

void Network::disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child) {
    const auto parentRef = Sim::Reference(parent.getOutputSimNode(), child.getInputSimNode(), parent.index);
    const auto childRef = Sim::Reference(child.getInputSimNode(), parent.getOutputSimNode(), child.index);
    sim->disconnect(parentRef, childRef);
}

Network::Network() : hsvColor(Util::random(), 0.8f, 0.65f), renderedColor(Util::hsv2rgb(this->hsvColor))  {

}

Network::Network(const glm::vec3 hsvColor) : hsvColor(hsvColor), renderedColor(Util::hsv2rgb(hsvColor)) {

}

Color Network::getRenderedColor() const {
    return this->renderedColor;
}

void Network::update() {
    this->notify({this});
}
