//
// Created by felix on 09.05.25.
//

#include "model/collision.hpp"

Collision::Collision(flecs::world &world)
        : world(world) {
    this->query = world.query<Models::Position, Models::Size>();
}

Collision::~Collision() {
    this->query.destruct();

}

flecs::entity_t Collision::getEntityBB(const Models::Position &position) const {
    flecs::entity_t entity;
    this->query.each([&](const flecs::entity& e, const Models::Position &pos, const Models::Size &size) {
        if (position.x >= pos.x && position.y >= pos.y &&
            position.x <= pos.x + size.x && position.y <= pos.y + size.y) {
            entity = e;
        }
    });
    return entity;
}
