//
// Created by felix on 02.06.25.
//

#ifndef CIRCUITBOARD_H
#define CIRCUITBOARD_H
#include "components.hpp"
#include "entt/entt.hpp"

namespace buildit {
struct component_key {
    std::string nmespace;
    std::string name;

    bool operator==(const component_key &) const = default;
};
} // namespace buildit

template<>
struct std::hash<buildit::component_key> {
    size_t operator()(const buildit::component_key &key) const noexcept {
        const size_t h1 = std::hash<std::string>{}(key.nmespace);
        const size_t h2 = std::hash<std::string>{}(key.name);
        return h1 ^ h2 << 1; // Combine hashes
    }
};

namespace buildit {

enum board_type { MAIN, MACRO };

class circuitboard {
public:
    explicit circuitboard(board_type type);

    circuitboard(board_type type, std::unique_ptr<ecs::registry> &reg);

    [[nodiscard]] ecs::registry &get_registry() const;

    [[nodiscard]] board_type get_type() const;

    void add_allowed_component(const component_key &key);

    void remove_allowed_component(const component_key &key);

private:
    const board_type type;
    const std::unique_ptr<ecs::registry> reg;
    std::unordered_set<component_key> allowed_components;
};

class circuitboard_registry {
    using sigh_type = entt::sigh<void(uint16_t, circuitboard &)>;

public:
    [[nodiscard]] circuitboard &get_board(uint16_t id) const;

    void create_circuitboard(board_type type);

    void register_circuitboard(uint16_t id, std::unique_ptr<circuitboard> &circuitboard);

    [[nodiscard]] entt::sink<sigh_type> on_create();

    [[nodiscard]] entt::sink<sigh_type> on_destroy();

private:
    uint16_t next_id = 0;
    std::unordered_map<uint16_t, std::unique_ptr<circuitboard> > boards;

    sigh_type create_signal;
    sigh_type destroy_signal;
};

using component_factory = void(circuitboard &, ecs::global_entity_t);

struct component {
    component_key key;
    std::function<component_factory> factory;
};

class component_registry {
    [[nodiscard]] component &get_component(const component_key &key);

    void register_component(std::unique_ptr<component> &component);

private:
    std::unordered_map<component_key, std::unique_ptr<component> > components;
};
} // namespace buildit

#endif //CIRCUITBOARD_H
