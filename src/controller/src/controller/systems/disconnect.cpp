//
// Created by felix on 01.05.25.
//

#include "controller/systems/disconnect.hpp"

#include "model/pinRenderCache.hpp"

using namespace Systems;

flecs::system disconnect(const flecs::world &world) {
    return world.system<Models::IsConnected>("Disconnect")
        .with<Models::Rotate>().or_().with<Models::Rotate>().parent().or_()
        .with<Models::Move>().or_().with<Models::Move>().parent()
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity e) {
            e.remove<Models::IsConnected>();
        });
}