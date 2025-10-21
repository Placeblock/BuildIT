//
// Created by felix on 21.10.25.
//

#include "simulation/simulation.hpp"
#include "buildit/plugin-api.h"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <ostream>

struct plugin_sim_node_t final : sim::node_t {
    explicit plugin_sim_node_t(api::simulation_chip_t *handle) : handle(handle) {

    }

    void update(const std::function<void(const sim::pin_t *pin)> &on_updated) override {
        this->handle->update(this->handle,
                             &on_updated,
                             [](const void *host_data, const void *sim_pin_impl) {
                                 (*static_cast<const std::function<void(const sim::pin_t *pin)> *>(
                                     host_data))(static_cast<const sim::pin_t *>(sim_pin_impl));
                             });
    }

private:
    api::simulation_chip_t *handle;
};

int main() {
    api::plugin_api_t api{};
    api.register_chip = [](api::register_chip_t &chip) {
        std::cout << "Registering chip " << chip.name << std::endl;
        const auto sim_chip = chip.create_simulation_chip(&chip);
        plugin_sim_node_t node{sim_chip};
    };
    api.create_simulation_pin = [](const api::pin_t &pin) {
        return static_cast<void *>(new sim::pin_t{pin.type, pin.value});
    };
    api.create_simulation_sink = [](const api::pin_sink_t &pin) {
        return static_cast<void *>(new sim::pin_sink_t{pin.type});
    };

    void *test_plugin = dlopen("../plugins/default/libplugin_default.so",
                               RTLD_NOW | RTLD_GLOBAL);
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(dlsym(
        test_plugin,
        "create_plugin"));
    api::plugin_t *plugin = create_plugin_fn();
    std::cout << "Loaded plugin " << plugin->name << " with version " << plugin->version <<
        std::endl;
    plugin->init(plugin, &api);
    plugin->shutdown(plugin, &api);
    plugin->destroy(plugin);

    return 0;
}