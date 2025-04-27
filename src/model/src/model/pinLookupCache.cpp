//
// Created by felix on 27.04.25.
//

#include "model/pinLookupCache.h"

void Models::PinLookupCache::use(const flecs::world &world) {
    this->observer = world.observer<Position, Pin, Rotation*>()
    .yield_existing()
    .event(flecs::OnAdd)
    .event(flecs::OnSet)
    .event(flecs::OnRemove)
    .each([this](const flecs::iter& it, size_t, const Position& pos, Pin& pin, const Rotation* rot) {
        Position absPos = pin.position;
        if (rot != nullptr) {
            rot->apply(absPos);
        }
        absPos += pos;
        if (it.event() == flecs::OnAdd) {
            this->addPin(&pin, absPos);
        } else if (it.event() == flecs::OnSet) {
            this->updatePin(&pin, absPos);
        } else {
            this->removePin(&pin);
        }
    });
}

void Models::PinLookupCache::addPin(Pin *pin, const Position &pos) {
    this->pins[pos] = pin;
}

void Models::PinLookupCache::removePin(Pin *pin) {
    const auto it = std::find_if(this->pins.begin(), this->pins.end(),
        [pin](const Pin *p) { return p == pin; });
    if (it != this->pins.end()) {
        this->pins.erase(it);
    }
}

void Models::PinLookupCache::updatePin(Pin *pin, const Position &pos) {
    this->removePin(pin);
    this->addPin(pin, pos);
}

Models::Pin * Models::PinLookupCache::getPin(const Position &pos) const {
    if (this->pins.find(pos) != this->pins.end()) {
        return this->pins.at(pos);
    }
    return nullptr;
}
