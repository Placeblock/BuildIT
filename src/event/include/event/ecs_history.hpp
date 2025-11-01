//
// Created by felix on 23.05.25.
//

#ifndef ECS_HISTORY_HPP
#define ECS_HISTORY_HPP
#include "common/player.hpp"
#include "ecs/collision.hpp"
#include "event.hpp"
#include "history.hpp"

namespace buildit::event {
class entity_ownership {
    entt::dense_map<player_id, ecs::global_entity_t> entities;

    player_id get_ownership(const ecs::global_entity_t &entity);

    void put_ownership(const ecs::global_entity_t &entity, player_id owner);
};

class registry_history final : public history<event_t> {
public:
    explicit registry_history(ecs::registry *reg) : reg(reg) {
    }

    void receive(std::unique_ptr<event_t> event, const std::function<void()> &runHandlers) override;

private:
    ecs::registry *reg;
};
} // namespace buildit::event
#endif //ECS_HISTORY_HPP
