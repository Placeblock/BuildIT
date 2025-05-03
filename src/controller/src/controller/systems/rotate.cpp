//
// Created by felix on 01.05.25.
//

#include "controller/systems/rotate.hpp"

#include "model/components.hpp"

using namespace Systems;

flecs::system rotate(const flecs::world &world) {
    return world.system<Models::Rotate, Models::Rotation>("Rotate")
    .kind(flecs::OnUpdate)
    .each([](const Models::Rotate &move, Models::Rotation &rotation) {
        rotation += move.delta;
    });
}
