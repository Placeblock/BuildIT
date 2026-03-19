#ifndef BUILDIT_MODULE_API_HPP
#define BUILDIT_MODULE_API_HPP
#include "ecs_history/history.hpp"
#include "ecs_history/static_entity.hpp"
#include "ecs_history/entt/change_mixin.hpp"
#include <memory>
#include <shared_mutex>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>


struct position_t {
    glm::vec2 pos;
};

template<>
struct entt::storage_type<position_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<position_t>;
};

template<typename Archive>
void serialize(Archive &archive, position_t &box) {
    archive(box.pos.x, box.pos.y);
}

struct gate_t {
};

template<>
struct entt::storage_type<gate_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<gate_t>;
};

template<typename Archive>
void serialize(Archive &archive, gate_t &box) {
}

namespace buildit::modules::api {

struct locked_registry_t {
    entt::registry &handle;
    ecs_history::static_entities_t &entities;
    std::vector<std::unique_ptr<ecs_history::base_storage_monitor_t> > &monitors;
    ecs_history::history_t &history;
    std::shared_mutex mutex;
};

}

#endif //BUILDIT_MODULE_API_HPP