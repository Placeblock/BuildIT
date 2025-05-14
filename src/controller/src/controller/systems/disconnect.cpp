//
// Created by felix on 01.05.25.
//

#include "controller/systems/disconnect.hpp"

#include "model/pinRenderCache.hpp"

using namespace Systems;

flecs::system disconnect(const flecs::world &world) {
    return world.system<models::IsConnected>("Disconnect")
        .with<models::Rotate>().or_().with<models::Rotate>().parent().or_()
        .with<models::move>().or_().with<models::move>().parent()
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity e) {
            e.remove<models::IsConnected>();
        });
}