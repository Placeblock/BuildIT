//
// Created by felix on 02.06.25.
//

#include "ecs/circuitboard.h"

using namespace buildit;

circuitboard::circuitboard(const board_type type) : type(type) {
}

circuitboard::circuitboard(const board_type type, std::unique_ptr<ecs::registry>& reg)
    : type(type), reg(std::move(reg)) {}

ecs::registry& circuitboard::get_registry() const {
    return *this->reg;
}

board_type circuitboard::get_type() const {
    return this->type;
}

void circuitboard::add_allowed_component(const component_key& key) {
    this->allowed_components.emplace(key);
}
void circuitboard::remove_allowed_component(const component_key& key) {
    this->allowed_components.erase(key);
}

circuitboard& circuitboard_registry::get_board(uint16_t id) const {
    if (!this->boards.contains(id)) {
        spdlog::error("Tried to access non existent board with id {}", id);
        throw std::runtime_error("Invalid board access");
    }
    return *this->boards.at(id);
}
void circuitboard_registry::create_circuitboard(const board_type type) {
    uint16_t id = this->next_id++;
    this->boards.emplace(id, std::make_unique<circuitboard>(type));
}
void circuitboard_registry::register_circuitboard(uint16_t id,
                                                  std::unique_ptr<circuitboard>& circuitboard) {
    this->boards.emplace(id, std::move(circuitboard));
}

entt::sink<circuitboard_registry::sigh_type> circuitboard_registry::on_create() {
    return entt::sink{this->create_signal};
}
entt::sink<circuitboard_registry::sigh_type> circuitboard_registry::on_destroy() {
    return entt::sink{this->destroy_signal};
}
void component_registry::register_component(std::unique_ptr<component>& component) {
    this->components.emplace(component->key, std::move(component));
}