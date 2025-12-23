//
// Created by felix on 21.10.25.
//


#include "buildit/plugin-api.hpp"
#include <functional>
#include <iostream>

// Plugin

using namespace buildit::api;

struct not_gate_sim_chip final : simulation_node_impl_t {
    pin_sink_t in;
    pin_t out;

    not_gate_sim_chip()
        : in{0, 0, 0},
          out{0, 0, 0, new bool{false}} {
        std::cout << "Creating not gate" << std::endl;
    }

    void update(const std::function<void(void *pin)> pin_updated_fn) override {
        if (*static_cast<bool *>(*in.pin_value) == *static_cast<bool *>(out.value)) {
            *static_cast<bool *>(out.value) = !*static_cast<bool *>(*in.pin_value);
            pin_updated_fn(out.sim_pin);
        }
    }

    pin_t *get_pins(size_t *count) override {
        *count = 1;
        return &out;
    }

    pin_sink_t *get_sinks(size_t *count) override {
        *count = 1;
        return &in;
    }
};

class default_plugin_t;

struct not_chip_type_t final : simulation_node_type_impl_t {
    explicit not_chip_type_t()
        : simulation_node_type_impl_t("de.codelix:and", 3, 3) {
    }

    [[nodiscard]] simulation_node_impl_t *create_node() const override {
        return new not_gate_sim_chip();
    }
};

class default_plugin_t final : public plugin_impl_t {
public:
    default_plugin_t() : plugin_impl_t("de.codelix:default", 0) {
    }

    void init(plugin_api_t *plugin_api) override {
        std::cout << "Hello World! From Plugin " << this->name << std::endl;
        std::cout << "Plugin API Version: " << plugin_api->version << std::endl;

        const auto chip_type = new not_chip_type_t();
        plugin_api->register_simulation_node_type(plugin_api, chip_type);
    }

    void shutdown(plugin_api_t *plugin_api) override {
        std::cout << "Bye World! From Plugin " << this->name << std::endl;
    }
};


#ifdef __cplusplus
extern "C" {
#endif

plugin_t *create_plugin() {
    auto *plugin = new default_plugin_t{};
    return plugin->handle();
};


#ifdef __cplusplus
}
#endif