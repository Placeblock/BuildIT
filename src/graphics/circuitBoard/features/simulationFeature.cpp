//
// Created by felix on 8/27/24.
//

#include "simulationFeature.h"

SimulationFeature::SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling,
                                     JointContainer *joints, NodeContainer *nodes)
        : simulation(sim), pinHandler(pinHandler), cabling(cabling),
          nodes(nodes), joints(joints) {
    this->nodes->Subject<NodeAddEvent>::subscribe(this);
    this->nodes->Subject<NodeRemoveEvent>::subscribe(this);
    this->joints->Subject<JointAddEvent>::subscribe(this);
    this->joints->Subject<JointRemoveEvent>::subscribe(this);
}

void SimulationFeature::checkNode(Node *node, glm::vec2 nodePos, bool disconnect) {
    for (const auto &iPin: node->inputPins) {
        Joint* joint = this->cabling->getJoint(nodePos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectChild(joint);
            } else {
                this->connectChild(joint, {node, node->getInputPinIndex(joint->getPos())});
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->cabling->getJoint(nodePos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                this->connectParent(joint, {node, node->getOutputPinIndex(joint->getPos())});
            }
        }
    }
}

void SimulationFeature::checkJoint(Joint *joint, glm::vec2 jointPos, bool disconnect) {
    Node* node = this->pinHandler->getNode(jointPos);
    if (node == nullptr) return;
    if (this->pinHandler->isInputPin(jointPos)) {
        if (disconnect) {
            this->disconnectChild(joint);
        } else {
            this->connectChild(joint, {node, node->getInputPinIndex(jointPos)});
        }
    }
    if (this->pinHandler->isOutputPin(jointPos)) {
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            this->connectParent(joint, {node, node->getOutputPinIndex(jointPos)});
        }
    }
}

void SimulationFeature::connectParent(Joint *joint, Pin parentPin) {
    for (const auto &childPin: joint->getNetwork()->childPins) {
        Network::connect(this->simulation, parentPin, childPin.second);
    }
    joint->getNetwork()->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    joint->getNetwork()->update();
}

void SimulationFeature::disconnectParent(Joint *joint) {
    for (const auto &childPin: joint->getNetwork()->childPins) {
        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, childPin.second);
    }
    joint->pin = {};
    joint->getNetwork()->parentPin = {};
    joint->getNetwork()->update();
}

void SimulationFeature::connectChild(Joint *joint, Pin childPin) {
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::connect(this->simulation, joint->getNetwork()->parentPin.second, childPin);
    }
    joint->pin = childPin;
    joint->getNetwork()->childPins[joint] = childPin;
}

void SimulationFeature::disconnectChild(Joint *joint) {
    joint->getNetwork()->childPins.erase(joint);
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, joint->pin);
    }
    joint->pin = {};
}

void SimulationFeature::notify(TypedSubject<ComponentAddEvent, Joint> *subject, const ComponentAddEvent &data) {
    Joint *joint = static_cast<Joint*>(data.component);
    this->checkJoint(joint, joint->getPos());
    joint->Movable::subscribe(static_cast<CastedObserver<MoveEvent, Joint>*>(this));
}

void SimulationFeature::notify(TypedSubject<ComponentRemoveEvent, Joint> *subject, const ComponentRemoveEvent &data) {
    Joint *joint = static_cast<Joint*>(data.component);
    this->checkJoint(joint, joint->getPos(), true);
    joint->Movable::unsubscribe(static_cast<CastedObserver<MoveEvent, Joint>*>(this));
}

void SimulationFeature::notify(TypedSubject<ComponentAddEvent, Node> *subject, const ComponentAddEvent &data) {
    Node *node = static_cast<Node*>(data.component);
    node->addToSimulation(this->simulation);
    this->checkNode(node, node->getPos());
    node->Movable::subscribe(static_cast<CastedObserver<MoveEvent, Node>*>(this));
    node->Rotatable::subscribe(this);
}

void SimulationFeature::notify(TypedSubject<ComponentRemoveEvent, Node> *subject, const ComponentRemoveEvent &data) {
    Node *node = static_cast<Node*>(data.component);
    node->removeFromSimulation(this->simulation);
    this->checkNode(node, node->getPos(), true);
    node->Movable::unsubscribe(static_cast<CastedObserver<MoveEvent, Node>*>(this));
    node->Rotatable::unsubscribe(this);
}

void SimulationFeature::notify(Node *node, const MoveEvent &data) {
    this->checkNode(node, node->getPos(), data.before);
}

void SimulationFeature::notify(Joint *joint, const MoveEvent &data) {
    this->checkJoint(joint, joint->getPos(), data.before);
}

void SimulationFeature::notify(Node *node, const RotateEvent &data) {
    this->checkNode(node, node->getPos(), data.before);
}
