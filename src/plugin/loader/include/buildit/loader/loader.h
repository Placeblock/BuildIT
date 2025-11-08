//
// Created by felix on 28.10.25.
//

#ifndef BUILDIT_LOADER_H
#define BUILDIT_LOADER_H
#include "api.h"
#include "../../../../../host/include/host/chip_type_registry.hpp"
#include <string>
#include <entt/entt.hpp>

#define LOADER_VERSION 1

namespace buildit::ecs {

struct plugin_graphics_component_t {
    void *data;
};

class plugin_chip_type_t final : public chip_type_t<
        loader::plugin_sim_node_t, plugin_graphics_component_t> {
public:
    explicit plugin_chip_type_t(const api::simulation_node_type_t *handle);

    plugin_chip_type_t(const plugin_chip_type_t &);

private:
    [[nodiscard]] loader::plugin_sim_node_t create_chip() const override;

    [[nodiscard]] plugin_graphics_component_t
    get_graphics(loader::plugin_sim_node_t *sim) const override;

    const api::simulation_node_type_t *handle;
};
}

namespace buildit::loader {
class loader_plugin_api_t final : public plugin_api_impl_t {
    entt::registry &reg;
    ecs::chip_type_registry_t &chip_type_registry;

public:
    explicit loader_plugin_api_t(entt::registry &reg,
                                 ecs::chip_type_registry_t &chip_type_registry);

    void register_simulation_node_type(api::simulation_node_type_t *chip_type) override;

    void *get_graphics_components(size_t *count) override;
};

class loader_t {
    loader_plugin_api_t plugin_api;
    std::vector<api::plugin_t *> plugins;

public:
    loader_t(entt::registry &reg, ecs::chip_type_registry_t &chip_type_registry);

    void load_plugins(const std::string &dir_path);

    void load_plugin(const std::string &file);

    void init_plugins();

private:
    static api::plugin_t *load_plugin_file(const std::string &file);
};

}


#endif //BUILDIT_LOADER_H