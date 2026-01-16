//
// Created by felix on 1/15/26.
//

#ifndef BUILDIT_MODULE_API_HPP
#define BUILDIT_MODULE_API_HPP
#include "ecs_history/gather_strategy/reactive/change_mixin.hpp"
#include <memory>
#include "modules/inifile.hpp"
#include <shared_mutex>
#include <entt/entt.hpp>
#include <glm/vec4.hpp>

#ifdef _WIN32
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT
#endif


struct bounding_box_t {
    glm::vec4 pos_size;
};

template<>
struct entt::storage_type<bounding_box_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<bounding_box_t>;
};

struct gate_t {
    uint32_t value;
};

template<>
struct entt::storage_type<gate_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<gate_t>;
};

namespace buildit::modules::api {

struct locked_registry_t {
    entt::registry &handle;
    std::shared_mutex mutex;
};

class module_t {
public:
    [[nodiscard]] virtual std::string get_name() const = 0;

    [[nodiscard]] virtual std::string get_description() const = 0;

    virtual void init(ini::IniFile &config) = 0;

    virtual void run(locked_registry_t &reg) = 0;

    virtual ~module_t() = default;
};

typedef std::unique_ptr<module_t> (*create_module_fn_t)();
}

#endif //BUILDIT_MODULE_API_HPP