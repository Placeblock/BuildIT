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
        if (this->deletedNetwork->parentReference.node != nullptr &&
            this->wire->network->parentReference.node == nullptr) {
            for (const auto &childRef: this->wire->network->childReferences) {
                Network::connect(this->simulation, this->deletedNetwork->parentReference, childRef);
            }
        }
        // We add all the old output References to the merged network
        for (auto iter = this->deletedNetwork->childReferences.begin(); iter != this->deletedNetwork->childReferences.end(); iter++) {
            this->wire->network->childReferences.insert(*iter);
        }
        for (auto &oldChildRef : this->deletedNetwork->childReferences) {
            this->wire->network->childReferences.insert(oldChildRef);
            // If the new merged network has an input reference we have to connect the output references!
            if (this->wire->network->parentReference.node != nullptr &&
                this->deletedNetwork->parentReference.node == nullptr) {
                Network::connect(this->simulation, this->wire->network->parentReference, oldChildRef);
            }
        }
        // We merge the deleted input reference into the merged network if necessary
        if (this->wire->network->parentReference.node == nullptr) {
            this->wire->network->parentReference = this->deletedNetwork->parentReference;
        }
        // Change networks
        for (const auto &item: this->deletedNetwork->joints) {
            item->network = this->wire->network;
            this->wire->network->joints.insert(item);
            wires->jointMap[item] = item->network;
        }
        for (const auto &item: this->deletedNetwork->wires) {
            item->network = this->wire->network;
            this->wire->network->wires.insert(item);
            wires->wireMap[item] = item->network;
        }
        // We don't remove the wires and jointVertexData from the old network to support rewind easily
        this->networkContainer->removeNetwork(this->deletedNetwork.get());
    }
    this->wireContainer->addWire(this->wire);
    Network::connect(this->wire.get());

    if (lastInBatch) this->regenerate();
}

void CreateWireAction::rewind(bool lastInBatch) {
    this->wireContainer->removeWire(this->wire.get());

    if (this->deletedNetwork) { // Split networks again
        for (const auto &delChildRef: this->deletedNetwork->childReferences) {
            // The parent reference belonged to the merged network, so we have to disconnect it from the deleted child reference
            if (this->deletedNetwork->parentReference.node == nullptr &&
                this->wire->network->parentReference.node != nullptr) {
                Network::disconnect(this->simulation, this->wire->network->parentReference, delChildRef);
            }
            this->wire->network->childReferences.erase(delChildRef);
        }
        // The input reference belonged to the deleted network, so we have to disconnect it from the original output references
        if (this->deletedNetwork->parentReference.node != nullptr &&
            this->wire->network->parentReference.node == nullptr) {
            for (const auto &childRef: this->wire->network->childReferences) {
                Network::disconnect(this->simulation, this->deletedNetwork->parentReference, childRef);
            }
        }
        // The input reference of the deleted network was merged, so we have to reset it
        if (this->deletedNetwork->parentReference.node != nullptr &&
            this->wire->network->parentReference.node != nullptr) {
            this->wire->network->parentReference = {};
        }
        for (const auto &vertex: this->deletedNetwork->joints) {
            vertex->network = this->deletedNetwork.get();
            this->wire->network->deleteJoint(vertex);
        }
        for (const auto &oldWire: this->deletedNetwork->wires) {
            oldWire->network = this->deletedNetwork.get();
            this->wire->network->deleteWire(oldWire, false);
        }
        wires->networks.insert(this->deletedNetwork);
    } else { // We have to resolve the new networks
        std::set<Joint*> data;
        data.insert(this->wire->start);
        data.insert(this->wire->end);
        NetworkResolver resolver{data};
        resolver.resolve();
        if (resolver.resolved.size() > 1) { // The new Network was split
            std::shared_ptr<Network> newNetwork = std::make_shared<Network>();  // The new newNetwork
            this->networkContainer->addNetwork(newNetwork);

            bool hasParent = this->wire->network->parentReference.node != nullptr;
            bool moveParentRef = resolver.resolved[1].contains(this->wire->network->parentReference.node);

            // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
            if (moveParentRef) {
                newNetwork->parentReference = this->wire->network->parentReference;
                for (const auto &childRef: this->wire->network->childReferences) {
                    if (!resolver.resolved[1].contains(childRef.first)) {
                        Network::disconnect(this->simulation, newNetwork->parentReference, childRef);
                    }
                }
            }

            for (const auto &vertex: resolver.resolved[1]) { // Update
                // The vertex was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
                if (vertex->network->childReferences.contains(vertex)) {
                    const auto childRef = vertex->network->childReferences[vertex];
                    newNetwork->childReferences.insert(childRef);
                    vertex->network->childReferences.erase(vertex);
                    if (hasParent && !moveParentRef) {
                        Network::disconnect(this->simulation, vertex->network->parentReference.second, childRef);
                    }
                }

                vertex->network->removeJoint(vertex);
                vertex->network = newNetwork.get();
                wires->jointMap[vertex] = newNetwork.get();
                newNetwork->joints.insert(vertex);
                for (const auto &vertexWire: vertex->wires) {
                    vertexWire->network->removeWire(vertexWire, false);
                    vertexWire->network = newNetwork.get();
                    wires->wireMap[vertexWire] = newNetwork.get();
                    newNetwork->wires.insert(vertexWire);
                }
            }
            if (moveParentRef) {
                this->wire->network->parentReference = {};
            }
        }
    }

    if (lastInBatch) this->regenerate();
}
