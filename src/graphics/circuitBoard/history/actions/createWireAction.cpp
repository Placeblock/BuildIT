//
// Created by felix on 8/3/24.
//

#include <set>
#include "createWireAction.h"
#include "graphics/circuitBoard/components/cabling/networkResolver.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"

void CreateWireAction::execute(bool lastInBatch) {
    this->wire->setNetwork(this->wire->start->getNetwork());
    if (this->wire->start->getNetwork() != this->wire->end->getNetwork()) { // We have to merge networks
        this->deletedNetwork = this->networkContainer->getOwningRef(this->wire->end->getNetwork());
        // If the deleted network has a parent reference we have to connect the child references
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->getNetwork()->parentPin.first == nullptr) {
            for (const auto &[_, childPin]: this->wire->getNetwork()->childPins) {
                Network::connect(this->simulation, this->deletedNetwork->parentPin.second, childPin);
            }
        }
        // We add all the old output References to the merged network
        for (auto &[oldChildJoint, oldChildPin] : this->deletedNetwork->childPins) {
            this->wire->getNetwork()->childPins[oldChildJoint] = oldChildPin;
            // If the new merged network has an input reference we have to connect the output references!
            if (this->wire->getNetwork()->parentPin.first != nullptr &&
                this->deletedNetwork->parentPin.first == nullptr) {
                Network::connect(this->simulation, this->wire->getNetwork()->parentPin.second, oldChildPin);
            }
        }
        // We merge the deleted input reference into the merged network if necessary
        if (this->wire->getNetwork()->parentPin.first == nullptr) {
            this->wire->getNetwork()->parentPin = this->deletedNetwork->parentPin;
        }
        // Change networks
        for (const auto &joint: this->deletedNetwork->joints) {
            Cabling::setNetwork(joint, this->wire->getNetwork());
        }
        for (const auto &delWire: this->deletedNetwork->wires) {
            Cabling::setNetwork(delWire, this->wire->getNetwork());
        }
        // We don't remove the wires and jointVertexData from the old network to support rewind easily
        this->networkContainer->removeNetwork(this->deletedNetwork.get());
    }
    this->wireContainer->addWire(this->wire);
    Network::connect(this->wire.get());
}

void CreateWireAction::rewind(bool lastInBatch) {
    this->wireContainer->removeWire(this->wire.get());

    if (this->deletedNetwork) { // Split networks again
        for (const auto &[oldChildJoint, oldChildPin]: this->deletedNetwork->childPins) {
            // The parent reference belonged to the merged network, so we have to disconnect it from the deleted child reference
            if (this->deletedNetwork->parentPin.first == nullptr &&
                this->wire->getNetwork()->parentPin.first != nullptr) {
                Network::disconnect(this->simulation, this->wire->getNetwork()->parentPin.second, oldChildPin);
            }
            this->wire->getNetwork()->childPins.erase(oldChildJoint);
        }
        // The input reference belonged to the deleted network, so we have to disconnect it from the original output references
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->getNetwork()->parentPin.first == nullptr) {
            for (const auto& [_, childPin]: this->wire->getNetwork()->childPins) {
                Network::disconnect(this->simulation, this->deletedNetwork->parentPin.second, childPin);
            }
        }
        // The input reference of the deleted network was merged, so we have to reset it
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->getNetwork()->parentPin.first != nullptr) {
            this->wire->getNetwork()->parentPin = {};
        }
        for (const auto &vertex: this->deletedNetwork->joints) {
            vertex->setNetwork(this->deletedNetwork.get());
            this->wire->getNetwork()->removeJoint(vertex);
        }
        for (const auto &oldWire: this->deletedNetwork->wires) {
            oldWire->setNetwork(this->deletedNetwork.get());
            this->wire->getNetwork()->removeWire(oldWire, false);
        }
        this->networkContainer->addNetwork(this->deletedNetwork);
    } else { // We have to resolve the new networks
        std::set<Joint*> data;
        data.insert(this->wire->start);
        data.insert(this->wire->end);
        NetworkResolver resolver{data};
        resolver.resolve();
        if (resolver.resolved.size() > 1) { // The new Network was split
            std::shared_ptr<Network> newNetwork = std::make_shared<Network>();  // The new newNetwork
            this->networkContainer->addNetwork(newNetwork);

            bool hasParent = this->wire->getNetwork()->parentPin.first != nullptr;
            bool moveParentRef = resolver.resolved[1].contains(this->wire->getNetwork()->parentPin.first);

            // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
            if (moveParentRef) {
                newNetwork->parentPin = this->wire->getNetwork()->parentPin;
                for (const auto &childRef: this->wire->getNetwork()->childPins) {
                    if (!resolver.resolved[1].contains(childRef.first)) {
                        Network::disconnect(this->simulation, newNetwork->parentPin.second, childRef.second);
                    }
                }
            }

            for (const auto &joint: resolver.resolved[1]) { // Update
                // The wire was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
                if (joint->getNetwork()->childPins.contains(joint)) {
                    const auto childRef = joint->getNetwork()->childPins[joint];
                    newNetwork->childPins[joint] = childRef;
                    joint->getNetwork()->childPins.erase(joint);
                    if (hasParent && !moveParentRef) {
                        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, childRef);
                    }
                }
                joint->getNetwork()->removeJoint(joint);
                Cabling::setNetwork(joint, newNetwork.get());
                for (const auto &jointWire: joint->wires) {
                    jointWire->getNetwork()->removeWire(jointWire, false);
                    Cabling::setNetwork(jointWire, newNetwork.get());
                }
            }
            if (moveParentRef) {
                this->wire->getNetwork()->parentPin = {};
            }
        }
    }
}
