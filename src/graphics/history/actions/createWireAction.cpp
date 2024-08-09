//
// Created by felix on 8/3/24.
//

#include <set>
#include "createWireAction.h"
#include "graphics/elements/wires/networkResolver.h"

CreateWireAction::CreateWireAction(std::shared_ptr<Wire> wire) {
    this->wire = wire;
}

void CreateWireAction::Execute(Wires *wires, WiresRenderer* renderer, bool regenerate) {
    this->wire->network = this->wire->start->network;
    if (this->wire->start->network != this->wire->end->network) { // We have to merge networks
        this->deletedNetwork = this->wire->end->network;
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
    this->wire->network->connect(this->wire);

    this->checkRegenerate(wires, renderer, regenerate);
}

void CreateWireAction::Rewind(Wires *wires, WiresRenderer* renderer, bool regenerate) {
    wires->deleteWire(this->wire);

    if (this->deletedNetwork) { // Split networks again
        for (const auto &item: this->deletedNetwork->vertices) {
            item->network = this->deletedNetwork;
        }
        for (const auto &item: this->deletedNetwork->wires) {
            item->network = this->deletedNetwork;
        }
        wires->networks.insert(this->deletedNetwork);
    } else { // We have to resolve the new networks
        std::set<std::shared_ptr<Vertex>> data;
        data.insert(this->wire->start);
        data.insert(this->wire->end);
        NetworkResolver resolver{data};
        resolver.resolve();
        if (resolver.resolved.size() > 1) { // The network was split
            std::shared_ptr<Network> network = std::make_shared<Network>();  // The new network
            wires->networks.insert(network);
            for (const auto &vertex: resolver.resolved[1]) { // Update network
                vertex->network->deleteVertex(vertex);
                vertex->network = network;
                wires->vertexMap[vertex] = network;
                network->vertices.insert(vertex);
                for (const auto &vertexWire: vertex->wires) {
                    vertexWire->network->deleteWire(vertexWire, false);
                    vertexWire->network = network;
                    wires->wireMap[vertexWire] = network;
                    network->wires.insert(vertexWire);
                }
            }
        }
    }

    this->checkRegenerate(wires, renderer, regenerate);
}
