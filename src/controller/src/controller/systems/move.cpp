//
// Created by felix on 01.05.25.
//

#include "controller/systems/move.hpp"

#include "model/components.hpp"

using namespace Systems;

flecs::system move(const flecs::world &world) {
    return world.system<models::move, models::Position>("Move")
        .kind(flecs::OnUpdate)
        .each([](const models::move &move, models::Position &position) {
            position += move.delta;
        });
}
