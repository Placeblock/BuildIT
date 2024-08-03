//
// Created by felix on 8/3/24.
//

#include <set>
#include "createWireAction.h"
#include "wires/networkResolver.h"

CreateWireAction::CreateWireAction(std::shared_ptr<Wire> wire) {
    this->wire = wire;
}

void CreateWireAction::Execute(Wires *wires) {
    this->wire->network = this->wire->start->network;
    if (this->wire->start->network != this->wire->end->network) { // We have to merge networks
        this->deletedNetwork = this->wire->end->network;
        for (const auto &item: this->deletedNetwork->vertices) {
            item->network = this->wire->network;
            wires->vertexMap[item] = item->network;
        }
        for (const auto &item: this->deletedNetwork->wires) {
            item->network = this->wire->network;
            wires->wireMap[item] = item->network;
        }
        // We don't remove the wires and vertices from the old network to support rewind easily
        wires->networks.erase(this->deletedNetwork);
    }
    wires->addWire(this->wire);
    this->wire->network->connect(this->wire);
}

void CreateWireAction::Rewind(Wires *wires) {
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
                vertex->network = network;
                for (const auto &vertexCable: vertex->wires) {
                    vertexCable->network = network;
                }
            }
        }
    }
}
