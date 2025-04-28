//
// Created by felix on 28.04.25.
//

#include "events/handler/moveEventHandler.hpp"

using namespace Events;

std::function<void(const MoveEvent &)> moveEventHandler(flecs::world &world) {
    world.component<Models::Position>();
    auto entity = world.entity()
        .set<Models::Position>({10, 20});

    return [world](const MoveEvent &event) {
        for (auto entity: event.entities) {
            auto pos2 = entity.get<Models::Position>();
        }
    };
}
