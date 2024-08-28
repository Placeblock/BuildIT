//
// Created by felix on 8/3/24.
//

#include <set>
#include "createWireAction.h"
#include "graphics/circuitBoard/components/wires/networkResolver.h"

void CreateWireAction::execute(bool lastInBatch) {
    this->wire->network = this->wire->start->network;
    if (this->wire->start->network != this->wire->end->network) { // We have to merge networks
        this->deletedNetwork = this->networkContainer->getOwningRef(this->wire->end->network);
        // If the deleted network has a parent reference we have to connect the child references
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->network->parentPin.first == nullptr) {
            for (const auto &[_, childPin]: this->wire->network->childPins) {
                Network::connect(this->simulation, this->deletedNetwork->parentPin.second, childPin);
            }
        }
        // We add all the old output References to the merged network
        for (auto &[oldChildJoint, oldChildPin] : this->deletedNetwork->childPins) {
            this->wire->network->childPins[oldChildJoint] = oldChildPin;
            // If the new merged network has an input reference we have to connect the output references!
            if (this->wire->network->parentPin.first != nullptr &&
                this->deletedNetwork->parentPin.first == nullptr) {
                Network::connect(this->simulation, this->wire->network->parentPin.second, oldChildPin);
            }
        }
        // We merge the deleted input reference into the merged network if necessary
        if (this->wire->network->parentPin.first == nullptr) {
            this->wire->network->parentPin = this->deletedNetwork->parentPin;
        }
        // Change networks
        for (const auto &joint: this->deletedNetwork->joints) {
            this->jointContainer->setNetwork(joint, this->wire->network);
        }
        for (const auto &delWire: this->deletedNetwork->wires) {
            this->wireContainer->setNetwork(delWire, this->wire->network);
        }
        // We don't remove the wires and jointVertexData from the old network to support rewind easily
        this->networkContainer->removeNetwork(this->deletedNetwork.get());
    }
    this->wireContainer->addWire(this->wire);
    Network::connect(this->wire.get());

    this->wiresRenderer->regenerateWires(this->wireContainer);
}

void CreateWireAction::rewind(bool lastInBatch) {
    this->wireContainer->removeWire(this->wire.get());

    if (this->deletedNetwork) { // Split networks again
        for (const auto &[oldChildJoint, oldChildPin]: this->deletedNetwork->childPins) {
            // The parent reference belonged to the merged network, so we have to disconnect it from the deleted child reference
            if (this->deletedNetwork->parentPin.first == nullptr &&
                this->wire->network->parentPin.first != nullptr) {
                Network::disconnect(this->simulation, this->wire->network->parentPin.second, oldChildPin);
            }
            this->wire->network->childPins.erase(oldChildJoint);
        }
        // The input reference belonged to the deleted network, so we have to disconnect it from the original output references
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->network->parentPin.first == nullptr) {
            for (const auto& [_, childPin]: this->wire->network->childPins) {
                Network::disconnect(this->simulation, this->deletedNetwork->parentPin.second, childPin);
            }
        }
        // The input reference of the deleted network was merged, so we have to reset it
        if (this->deletedNetwork->parentPin.first != nullptr &&
            this->wire->network->parentPin.first != nullptr) {
            this->wire->network->parentPin = {};
        }
        for (const auto &vertex: this->deletedNetwork->joints) {
            vertex->network = this->deletedNetwork.get();
            this->wire->network->removeJoint(vertex);
        }
        for (const auto &oldWire: this->deletedNetwork->wires) {
            oldWire->network = this->deletedNetwork.get();
            this->wire->network->removeWire(oldWire, false);
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

            bool hasParent = this->wire->network->parentPin.first != nullptr;
            bool moveParentRef = resolver.resolved[1].contains(this->wire->network->parentPin.first);

            // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
            if (moveParentRef) {
                newNetwork->parentPin = this->wire->network->parentPin;
                for (const auto &childRef: this->wire->network->childPins) {
                    if (!resolver.resolved[1].contains(childRef.first)) {
                        Network::disconnect(this->simulation, newNetwork->parentPin.second, childRef.second);
                    }
                }
            }

            for (const auto &joint: resolver.resolved[1]) { // Update
                // The joint was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
                if (joint->network->childPins.contains(joint)) {
                    const auto childRef = joint->network->childPins[joint];
                    newNetwork->childPins[joint] = childRef;
                    joint->network->childPins.erase(joint);
                    if (hasParent && !moveParentRef) {
                        Network::disconnect(this->simulation, joint->network->parentPin.second, childRef);
                    }
                }
                std::shared_ptr<Joint> owningRef = this->jointContainer->getOwningRef(joint);
                this->jointContainer->removeJoint(joint);
                joint->network = newNetwork.get();
                this->jointContainer->addJoint(owningRef);
                for (const auto &jointWire: joint->wires) {
                    jointWire->network->removeWire(jointWire, false);
                    this->wireContainer->setNetwork(jointWire, newNetwork.get());
                }
            }
            if (moveParentRef) {
                this->wire->network->parentPin = {};
            }
        }
    }

    this->wiresRenderer->regenerateWires(this->wireContainer);
}
