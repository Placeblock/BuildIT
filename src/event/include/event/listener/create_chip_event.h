//
// Created by felix on 10/25/25.
//

#ifndef BUILDIT_CREATE_CHIP_EVENT_H
#define BUILDIT_CREATE_CHIP_EVENT_H
#include "ecs/chip_type_registry_t.hpp"
#include "event/event_t.hpp"
#include <entt/entt.hpp>
#include <string>
#include <glm/vec2.hpp>

struct create_chip_event_t final : buildit::event::event_t {
    const std::string key;
    const glm::vec2 position;
    explicit create_chip_event_t(const uint32_t *player_id, std::string key, glm::vec2 position);
};

class create_chip_event_handler_t {
    const chip_type_registry_t &component_type_registry;
    entt::registry &registry;

public:
    explicit create_chip_event_handler_t(const chip_type_registry_t &component_type_registry, entt::registry &registry);

private:
    void on(const create_chip_event_t& event) const ;
};

#endif //BUILDIT_CREATE_CHIP_EVENT_H
