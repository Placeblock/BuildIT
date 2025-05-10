//
// Created by felix on 09.05.25.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <flecs.h>

#include "components.hpp"

class Collision {
    flecs::world &world;
    flecs::query<Models::Position, Models::Size> query;
public:
    explicit Collision(flecs::world &world);
    ~Collision();

    [[nodiscard]] flecs::entity_t getEntityBB(const Models::Position& position) const;
};

#endif //COLLISION_HPP
