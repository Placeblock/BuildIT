//
// Created by felix on 9/7/24.
//

#include <ranges>
#include "networkContainer.h"

void NetworkContainer::addNetwork(const std::shared_ptr<Network>& network) {
    this->networks.push_back(network);
    this->Subject<NetworkAddEvent>::notify({network.get()});
}

void NetworkContainer::removeNetwork(Network *network) {
    const auto iter = std::ranges::find_if(this->networks, [&network](const std::shared_ptr<Network>& j){
        return j.get() == network;
    });
    assert(iter != this->networks.end() && "Tried to remove non existent network from networks");
    this->networks.erase(iter);
    this->Subject<NetworkRemoveEvent>::notify({network});
}

std::shared_ptr<Network> NetworkContainer::getOwningRef(const Network *network) const {
    const auto iter = std::ranges::find_if(this->networks, [&network](const std::shared_ptr<Network>& j){
        return j.get() == network;
    });
    if (iter == this->networks.end()) return nullptr;
    return *iter;
}