//
// Created by felix on 25.04.25.
//

#include "model/pinCache.hpp"

using namespace Models;

void PinCache::use(const flecs::world &world) {
    this->observer = world.observer<Pin>()
        .event(flecs::OnSet)
        .each([](flecs::entity e, Pin& pin) {
            this->pins.
        });
}
