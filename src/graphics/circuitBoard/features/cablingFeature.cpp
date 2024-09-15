//
// Created by felix on 8/31/24.
//

#include "cablingFeature.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"

CablingFeature::CablingFeature(History *history,
                               Subject<ComponentAddEvent> *addSubject,
                               Subject<ComponentRemoveEvent> *removeSubject,
                               CablingRenderer *cablingRenderer)
                               : history(history), cablingRenderer(cablingRenderer) {
    this->wires.Subject<WireAddEvent>::subscribe(this);
    this->wires.Subject<WireRemoveEvent>::subscribe(this);
    removeSubject->subscribe(this);
    addSubject->subscribe(&this->cabling);
    removeSubject->subscribe(&this->cabling);
    this->wires.Subject<WireAddEvent>::subscribe(&this->cabling);
    this->wires.Subject<WireRemoveEvent>::subscribe(&this->cabling);
}

void CablingFeature::notify(const ComponentRemoveEvent &data) {
    if (Joint *joint = dynamic_cast<Joint*>(data.component)) {
        for (const auto &wire: joint->wires) {
            std::shared_ptr<Wire> owningRef = this->wires.getOwningRef(wire);
            std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(&this->wires,
                                                                                      owningRef,
                                                                                      true);
            History::dispatch(this->history, dAction);
        }
        if (joint->getNetwork()->wires.empty() && joint->getNetwork()->joints.empty()) {
            this->networks.removeNetwork(joint->getNetwork());
        }
    }
}

void CablingFeature::notify(const WireAddEvent &data) {
    Wire *wire = data.wire;
    wire->setNetwork(wire->start->getNetwork());
    if (wire->start->getNetwork() != wire->end->getNetwork()) { // We have to merge networks
        Network *deletedNetwork = wire->end->getNetwork();
        this->Subject<NetworksMergeEvent>::notify(NetworksMergeEvent{wire->start->getNetwork(), deletedNetwork});

        // We add all the old output References to the merged network
        for (auto &[oldChildJoint, oldChildPin] : deletedNetwork->childPins) {
            wire->getNetwork()->childPins[oldChildJoint] = oldChildPin;
        }
        // We merge the deleted input reference into the merged network if necessary
        if (wire->getNetwork()->parentPin.first == nullptr) {
            wire->getNetwork()->parentPin = deletedNetwork->parentPin;
        }
        // Change networks
        for (const auto &joint: deletedNetwork->joints) {
            joint->setNetwork(wire->getNetwork());
            wire->getNetwork()->joints.push_back(joint);
        }
        for (const auto &delWire: deletedNetwork->wires) {
            delWire->setNetwork(wire->getNetwork());
            wire->getNetwork()->wires.push_back(delWire);
        }
        // We don't remove the wires and jointVertexData from the old network to support rewind easily
        this->networks.removeNetwork(deletedNetwork);
    }
    this->cablingRenderer->addWire(wire, true);
}

void CablingFeature::notify(const WireRemoveEvent &data) {
    Wire *wire = data.wire;
    this->cablingRenderer->removeWire(wire, true);
    std::set<Joint*> resolverData;
    resolverData.insert(wire->start);
    resolverData.insert(wire->end);
    NetworkResolver resolver{resolverData};
    resolver.resolve();
    if (resolver.resolved.size() > 1) { // The new Network was split

        std::shared_ptr<Network> newNetwork = std::make_shared<Network>();  // The new newNetwork
        this->networks.addNetwork(newNetwork);

        bool moveParentRef = resolver.resolved[1].contains(wire->getNetwork()->parentPin.first);

        // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
        if (moveParentRef) {
            newNetwork->parentPin = wire->getNetwork()->parentPin;
        }

        for (const auto &joint: resolver.resolved[1]) { // Update
            // The wire was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
            if (joint->getNetwork()->childPins.contains(joint)) {
                const auto childRef = joint->getNetwork()->childPins[joint];
                newNetwork->childPins[joint] = childRef;
                joint->getNetwork()->childPins.erase(joint);
            }
            joint->getNetwork()->removeJoint(joint);
            newNetwork->joints.push_back(joint);

            joint->setNetwork(newNetwork.get());
            for (const auto &jointWire: joint->wires) {
                jointWire->getNetwork()->removeWire(jointWire);
                newNetwork->wires.push_back(jointWire);
                jointWire->setNetwork(newNetwork.get());
            }
        }
        if (moveParentRef) {
            wire->getNetwork()->parentPin = {};
        }

        std::unordered_set<Network*> newNetworks{wire->getNetwork(), newNetwork.get()};
        this->Subject<NetworksSplitEvent>::notify({wire->getNetwork(), newNetworks});
    }
}
