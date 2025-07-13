//
// Created by felix on 20.04.25.
//

#include "entt/entt.hpp"

struct Pin {
    int x, y;
};
struct Door {
    int x, y, z;
};

int main() {
    entt::registry registry;

    const auto entity = registry.create();

    registry.emplace<Position>(entity, 0, 0);
    registry.emplace<Velocity>(entity, 2, 2);

    for (auto entity : registry.view<Pin>()) {
        auto &pos = registry.get<Position>(entity);
        auto &vel = registry.get<Velocity>(entity);
        pos.x = pos.x + vel.x;
        pos.y = pos.y + vel.y;
    }

    return 1;
}
