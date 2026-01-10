//
// Created by felix on 20.04.25.
//

#include "entt/entt.hpp"
#include "ecs/registry_node.hpp"
#include "ecs_history/gather_strategy/change_mixin.hpp"

struct gate_t {
    uint32_t x, y;
};


template<>
struct entt::storage_type<gate_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<gate_t>;
};

template<typename T>
void emplace(entt::registry &registry, const entt::entity entt, const T &value) {
    registry.emplace_or_replace<T>(entt, value);
}

int main() {

    return 1;
}
