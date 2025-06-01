//
// Created by felix on 02.06.25.
//

#ifndef CIRCUITBOARD_H
#define CIRCUITBOARD_H
#include "components.hpp"
#include "entt/entt.hpp"

namespace buildit::ecs {

enum board_type { MAIN, MACRO };

class circuitboard {
public:
    circuitboard(board_type type);
    circuitboard(board_type type, std::unique_ptr<registry>& reg);

    [[nodiscard]] registry& get_registry() const;
    [[nodiscard]] board_type get_type() const;

private:
    const board_type type;
    const std::unique_ptr<registry> reg;
};

class circuitboard_registry {
    using sigh_type = entt::sigh<void(uint16_t, circuitboard&)>;

public:
    [[nodiscard]] circuitboard& get_board(uint16_t id) const;

    [[nodiscard]] entt::sink<sigh_type> on_create();
    [[nodiscard]] entt::sink<sigh_type> on_destroy();

private:
    std::unordered_map<uint16_t, std::unique_ptr<circuitboard>> boards;

    sigh_type create_signal;
    sigh_type destroy_signal;
};

} // namespace buildit::ecs

#endif //CIRCUITBOARD_H
