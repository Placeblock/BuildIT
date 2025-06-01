//
// Created by felix on 02.06.25.
//

#include "ecs/circuitboard.h"

using namespace buildit::ecs;

circuitboard::circuitboard(const board_type type) : type(type), reg(std::make_unique<>{}) {}
circuitboard::circuitboard(const board_type type, std::unique_ptr<registry>& reg)
    : type(type), reg(std::move(reg)) {}

registry& circuitboard::get_registry() const {
    return *reg;
}

board_type circuitboard::get_type() const {
    return type;
}

circuitboard& circuitboard_registry::get_board(uint16_t id) const {
    if (!this->boards.contains(id)) {
        spdlog::error("Tried to access non existent board with id {}", id);
        throw std::runtime_error("Invalid board access");
    }
    return *this->boards.at(id);
}
entt::sink<circuitboard_registry::sigh_type> circuitboard_registry::on_create() {
    return entt::sink{this->create_signal};
}
entt::sink<circuitboard_registry::sigh_type> circuitboard_registry::on_destroy() {
    return entt::sink{this->destroy_signal};
}