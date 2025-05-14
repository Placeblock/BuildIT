//
// Created by felix on 09.05.25.
//

#include "model/collision.hpp"

using namespace Model;

collision::collision(BuildIT::Registry& registry)
    : registry(registry)
{}

BuildIT::Entity collision::getEntityBB(const Model::Position &position) const
{
    const auto& view = this->registry.view<Model::Position, Model::Size>();
    for(auto [entity, pos, size]: view.each()) {
        if (position.x >= pos.x && position.y >= pos.y &&
            position.x <= pos.x + size.x && position.y <= pos.y + size.y) {
            return entity;
        }
    }
    return BuildIT::Entity{};
}
