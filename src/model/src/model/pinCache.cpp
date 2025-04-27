//
// Created by felix on 25.04.25.
//

#include "model/pinCache.hpp"

using namespace Models;

void PinCache::use(const flecs::world &world) {
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

void PinCache::addPin(Pin *pin, const Position& pos) {
    this->pins.push_back(CachedPin{pin, pos+pin->position});
}

void PinCache::removePin(Pin *pin) {
    const auto it = std::find_if(this->pins.begin(), this->pins.end(), [pin](const CachedPin& cached) {
        return cached.pin == pin;
    });
    if (it != this->pins.end()) {
        this->pins.erase(it);
    }
}

void PinCache::updatePin(Pin *pin, const Position& pos) {
    const auto it = std::find_if(this->pins.begin(), this->pins.end(), [pin](const CachedPin& cached) {
        return cached.pin == pin;
    });
    if (it != this->pins.end()) {
        it->position = pos;
    }
}
