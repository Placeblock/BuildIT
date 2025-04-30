//
// Created by felix on 01.05.25.
//

#include "model/systems/move.hpp"

#include "model/components.hpp"

using namespace Systems;

flecs::system move(const flecs::world &world) {
    return world.system<Models::Move, Models::Position>("Move")
        .kind(flecs::OnUpdate)
        .each([](const Models::Move &move, Models::Position &position) {
            position += move.delta;
        });
}
