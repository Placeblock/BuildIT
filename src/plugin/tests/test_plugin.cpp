//
// Created by felix on 21.10.25.
//

#include "simulation/simulation.hpp"
#include "buildit/plugin-api.h"
#include "simulation/graph.hpp"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

struct plugin_sim_node_t final : sim::node_t {
    explicit plugin_sim_node_t(api::chip_t *handle) : handle(handle) {

    }

    void update(const std::function<void(const sim::pin_t *pin)> &on_updated) override {
        this->handle->update(this->handle,
                             &on_updated,
                             [](const void *host_data, const void *sim_pin_impl) {
                                 (*static_cast<const std::function<void(const sim::pin_t *pin)> *>(
                                     host_data))(static_cast<const sim::pin_t *>(sim_pin_impl));
                             });
    }

    std::vector<sim::pin_t> pins;
    std::vector<sim::pin_sink_t> sinks;

private:
    api::chip_t *handle;
};

std::unique_ptr<plugin_sim_node_t> node;

long long updates = 0;

[[noreturn]] void measure() {
    long long last_value = updates;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const long long value = updates;
        std::cout << value - last_value << " updates/s\n";
        last_value = value;
    }
}

api::chip_t *chip;

int main() {

    api::plugin_api_t api{};
    api.register_chip_type = [](api::chip_type_t &chip_type) {
        std::cout << "Registering chip " << chip_type.name << std::endl;
        const auto sim_chip = chip_type.create_chip(&chip_type);
        chip = sim_chip;
        node = std::make_unique<plugin_sim_node_t>(sim_chip);

        size_t pin_count = 0;
        api::pin_t *pins = sim_chip->get_pins(sim_chip, &pin_count);
        for (int i = 0; i < pin_count; ++i) {
            node->pins.emplace_back(pins[i].type, pins[i].value);
            pins[i].sim_pin = &node->pins.back();
        }
        size_t sink_count = 0;
        api::pin_sink_t *sinks = sim_chip->get_sinks(sim_chip, &sink_count);
        for (int i = 0; i < sink_count; ++i) {
            node->sinks.emplace_back(sinks[i].type);
            sinks[i].pin_value = &node->sinks.back().pin_value;
        }
        std::cout << "Created chip" << std::endl;
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

    sim::simulation_t simulation;
    sim::graph::connect(node->pins[0], *node, node->sinks[0]);

    std::thread monitor_thread(measure);

    simulation.update(node.get());
    while (!simulation.is_empty()) {
        simulation.poll_and_update();
        updates++;
    }

    plugin->shutdown(plugin, &api);
    plugin->destroy(plugin);

    return 0;
}