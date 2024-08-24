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
        // If the deleted network has an input reference we have to connect the output references
        if (this->deletedNetwork->parentReference.first != nullptr &&
            this->wire->network->parentReference.first == nullptr) {
            for (const auto &outReference: this->wire->network->childReferences) {
                this->simulation->connect(this->deletedNetwork->inputReference.second, outReference.second);
            }
        }
        // We add all the old output References to the merged network
        for (const auto &oldOutReference: this->deletedNetwork->childReferences) {
            this->wire->network->childReferences.insert(oldOutReference);
            // If the new merged network has an input reference we have to connect the output references!
            if (this->wire->network->parentReference.first != nullptr &&
                this->deletedNetwork->parentReference.first == nullptr) {

                this->simulation->connect(this->wire->network->inputReference.second, oldOutReference.second);
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
    this->wire->network->connect(this->wire.get());

    if (lastInBatch) this->regenerate();
}

void CreateWireAction::rewind(bool lastInBatch) {
    wires->deleteWire(this->wire.get());

    if (this->deletedNetwork) { // Split networks again
        for (const auto &oldOutRef: this->deletedNetwork->childReferences) {
            this->wire->network->childReferences.erase(oldOutRef.first);
            // The input reference belonged to the merged network, so we have to disconnect it from the deleted output reference
            if (this->deletedNetwork->parentReference.first == nullptr &&
                this->wire->network->parentReference.first != nullptr) {
                this->simulation->disconnect(this->wire->network->inputReference.second, oldOutRef.second);
            }
        }
        // The input reference belonged to the deleted network, so we have to disconnect it from the original output references
        if (this->deletedNetwork->parentReference.first != nullptr &&
            this->wire->network->parentReference.first == nullptr) {
            for (const auto &outRef: this->wire->network->childReferences) {
                this->simulation->disconnect(this->deletedNetwork->inputReference.second, outRef.second);
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

            bool moveInputReference = false;
            for (const auto &vertex: resolver.resolved[1]) {
                if (vertex->network->parentReference.first == vertex) {
                    moveInputReference = true;
                }
            }
            for (const auto &vertex: resolver.resolved[1]) { // Update
                // The vertex was an output reference, so we move the reference and disconnect it from the input reference if it won't get moved
                if (vertex->network->childReferences.contains(vertex)) {
                    const auto outRef = vertex->network->childReferences[vertex];
                    newNetwork->childReferences[vertex] = outRef;
                    vertex->network->childReferences.erase(vertex);
                    if (!moveInputReference) {
                        this->simulation->disconnect(vertex->network->inputReference.second, outRef);
                    }
                }
                // The vertex of the new network was an input reference, so we have to move the reference and disconnect output references
                if (vertex->network->parentReference.first == vertex) {
                    newNetwork->parentReference = vertex->network->parentReference;
                    vertex->network->parentReference = {};
                    for (const auto &outRef: vertex->network->childReferences) {
                        this->simulation->disconnect(newNetwork->inputReference.second, outRef.second);
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
        }
    }

    if (lastInBatch) this->regenerate();
}
