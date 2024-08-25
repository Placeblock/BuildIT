//
// Created by felix on 8/3/24.
//

#include <set>
#include "createWireAction.h"
#include "graphics/circuitBoard/elements/wires/networkResolver.h"

void CreateWireAction::execute(bool lastInBatch) {
    this->wire->network = this->wire->start->network;
    if (this->wire->start->network != this->wire->end->network) { // We have to merge networks
        this->deletedNetwork = this->wires->getOwningRef(this->wire->end->network);
        // If the deleted network has a parent reference we have to connect the child references
        if (this->deletedNetwork->parentReference.first != nullptr &&
            this->wire->network->parentReference.first == nullptr) {
            for (const auto &childRef: this->wire->network->childReferences) {
                Network::connect(this->deletedNetwork->parentReference.second, childRef.second);
            }
        }
        // We add all the old output References to the merged network
        for (const auto &oldChildRef: this->deletedNetwork->childReferences) {
            this->wire->network->childReferences.insert(oldChildRef);
            // If the new merged network has an input reference we have to connect the output references!
            if (this->wire->network->parentReference.first != nullptr &&
                this->deletedNetwork->parentReference.first == nullptr) {
                Network::connect(this->wire->network->parentReference.second, oldChildRef.second);
            }
        }
        // We merge the deleted input reference into the merged network if necessary
        if (this->wire->network->parentReference.first == nullptr) {
            this->wire->network->parentReference = this->deletedNetwork->parentReference;
        }
        // Change networks
        for (const auto &item: this->deletedNetwork->vertices) {
            item->network = this->wire->network;
            this->wire->network->vertices.insert(item);
            wires->vertexMap[item] = item->network;
        }
        for (const auto &item: this->deletedNetwork->wires) {
            item->network = this->wire->network;
            this->wire->network->wires.insert(item);
            wires->wireMap[item] = item->network;
        }
        // We don't remove the wires and vertexData from the old network to support rewind easily
        wires->networks.erase(this->deletedNetwork);
    }
    wires->addWire(this->wire);
    Network::connect(this->wire.get());

    if (lastInBatch) this->regenerate();
}

void CreateWireAction::rewind(bool lastInBatch) {
    wires->deleteWire(this->wire.get());

    if (this->deletedNetwork) { // Split networks again
        for (const auto &delChildRef: this->deletedNetwork->childReferences) {
            // The parent reference belonged to the merged network, so we have to disconnect it from the deleted child reference
            if (this->deletedNetwork->parentReference.first == nullptr &&
                this->wire->network->parentReference.first != nullptr) {
                Network::disconnect(this->wire->network->parentReference.second, delChildRef.second);
            }
            this->wire->network->childReferences.erase(delChildRef.first);
        }
        // The input reference belonged to the deleted network, so we have to disconnect it from the original output references
        if (this->deletedNetwork->parentReference.first != nullptr &&
            this->wire->network->parentReference.first == nullptr) {
            for (const auto &childRef: this->wire->network->childReferences) {
                Network::disconnect(this->deletedNetwork->parentReference.second, childRef.second);
            }
        }
        // The input reference of the deleted network was merged, so we have to reset it
        if (this->deletedNetwork->parentReference.first != nullptr &&
            this->wire->network->parentReference.first != nullptr) {
            this->wire->network->parentReference = {};
        }
        for (const auto &vertex: this->deletedNetwork->vertices) {
            vertex->network = this->deletedNetwork.get();
            this->wire->network->deleteVertex(vertex);
        }
        for (const auto &oldWire: this->deletedNetwork->wires) {
            oldWire->network = this->deletedNetwork.get();
            this->wire->network->deleteWire(oldWire, false);
        }
        wires->networks.insert(this->deletedNetwork);
    } else { // We have to resolve the new networks
        std::set<Vertex*> data;
        data.insert(this->wire->start);
        data.insert(this->wire->end);
        NetworkResolver resolver{data};
        resolver.resolve();
        if (resolver.resolved.size() > 1) { // The new Network was split
            std::shared_ptr<Network> newNetwork = std::make_shared<Network>();  // The new newNetwork
            wires->networks.insert(newNetwork);

            bool hasParent = this->wire->network->parentReference.first != nullptr;
            bool moveParentRef = resolver.resolved[1].contains(this->wire->network->parentReference.first);

            // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
            if (moveParentRef) {
                newNetwork->parentReference = this->wire->network->parentReference;
                for (const auto &childRef: this->wire->network->childReferences) {
                    Network::disconnect(newNetwork->parentReference.second, childRef.second);
                }
            }

            for (const auto &vertex: resolver.resolved[1]) { // Update
                // The vertex was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
                if (vertex->network->childReferences.contains(vertex)) {
                    const auto childRef = vertex->network->childReferences[vertex];
                    newNetwork->childReferences[vertex] = childRef;
                    vertex->network->childReferences.erase(vertex);
                    if (hasParent && !moveParentRef) {
                        Network::disconnect(vertex->network->parentReference.second, childRef);
                    }
                }

                vertex->network->deleteVertex(vertex);
                vertex->network = newNetwork.get();
                wires->vertexMap[vertex] = newNetwork.get();
                newNetwork->vertices.insert(vertex);
                for (const auto &vertexWire: vertex->wires) {
                    vertexWire->network->deleteWire(vertexWire, false);
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
