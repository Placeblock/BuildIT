//
// Created by felix on 01.05.25.
//

#include "model/systems/disconnect.hpp""

#include "model/pinRenderCache.hpp"

flecs::system Systems::disconnect(const flecs::world &world) {
    return world.system<Models::Move>("Disconnect")
        .with<Models::Rotate>().or_()
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity e) {
            e.each([](const flecs::id id) {
                if (id.first() == flecs::type<Models::Pin>()) {
                    flecs::entity pin = id.second();
                }
            });
        });
}
