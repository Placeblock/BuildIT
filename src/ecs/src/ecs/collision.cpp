//
// Created by felix on 09.05.25.
//

#include "ecs/collision.hpp"

using namespace buildit::ecs;

collision::collision(registry &reg) : reg(reg) {
}

entt::entity collision::getEntityBB(const glm::vec2 &pos) const {
    const auto &view = this->reg.view<position_t, size_comp_t>();
    for (auto [entity, pos2, size] : view.each()) {
        if (pos.x >= pos2.pos.x && pos.y >= pos2.pos.y && pos.x <= pos2.pos.x + size.size.x
            && pos.y <= pos2.pos.y + size.size.y) {
            return entity;
        }
    }
    return entt::tombstone;
}
