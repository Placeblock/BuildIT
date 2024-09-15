//
// Created by felix on 9/7/24.
//

#include "wireContainer.h"

void WireContainer::addWire(const std::shared_ptr<Wire>& wire) {
    this->wires.push_back(wire);
    this->Subject<WireAddEvent>::notify({wire.get()});
}

void WireContainer::removeWire(Wire *wire) {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    assert(iter != this->wires.end() && "Tried to remove non existent wire from wires");
    this->wires.erase(iter);
    this->Subject<WireRemoveEvent>::notify({wire});
}

size_t WireContainer::getWireIndex(const Wire* wire) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    return std::distance(this->wires.begin(), iter);
}

std::shared_ptr<Wire> WireContainer::getOwningRef(const Wire *wire) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    if (iter == this->wires.end()) return nullptr;
    return *iter;
}

std::set<const Wire *> WireContainer::getWires() const {
    std::set<const Wire*> nOWires;
    std::transform(this->wires.begin(), this->wires.end(), std::inserter(nOWires, nOWires.end()), [](const auto& w) {
        return w.get();
    });
    return nOWires;
}