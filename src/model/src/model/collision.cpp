//
// Created by felix on 09.05.25.
//

#include "model/collision.hpp"

using namespace buildit::ecs;

collision::collision(registry& reg) : reg(reg) {}

entity collision::getEntityBB(const position& pos) const {
    const auto& view = this->reg.view<position, size>();
    for (auto [entity, pos2, size] : view.each()) {
        if (pos.x >= pos2.x && pos.y >= pos2.y && pos.x <= pos2.x + size.x
            && pos.y <= pos2.y + size.y) {
            return entity;
        }
    }
    return entt::tombstone;
}
