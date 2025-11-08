//
// Created by felix on 10/25/25.
//

#include "create_chip_event.h"

#include "ecs/components.hpp"
#include <utility>

#include <entt/entt.hpp>

using namespace buildit::event;

create_chip_event_t::create_chip_event_t(const uint32_t *player_id,
                                         std::string key,
                                         const glm::vec2 position)
    : event_t(player_id), key(std::move(key)), position(position) {
}

create_chip_event_handler_t::create_chip_event_handler_t(
    const ecs::chip_type_registry_t &component_type_registry,
    entt::registry &registry)
    : component_type_registry(component_type_registry), registry(registry) {
}

void create_chip_event_handler_t::on(const create_chip_event_t &event) const {
    const ecs::base_chip_type_t *chip_type = this->component_type_registry.get_chip_type(event.key);

    if (chip_type == nullptr) {
        spdlog::error("Tried to instantiate unknown chip type '{}'", event.key);
        throw std::runtime_error("unknown chip type");
    }

    const entt::entity entity = this->registry.create();

    sim::node_t &sim_node = chip_type->create_sim_component(this->registry, entity);
    chip_type->update_graphics_component(this->registry, entity);
    this->registry.emplace<ecs::position_t>(entity, event.position);
}