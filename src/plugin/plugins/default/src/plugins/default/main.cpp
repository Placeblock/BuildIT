//
// Created by felix on 21.10.25.
//


#include "buildit/plugin-api.hpp"
#include <functional>
#include <iostream>

// Plugin

struct not_gate_sim_chip final : simulation_chip_impl_t {
    api::pin_sink_t in;
    api::pin_t out;
    void *host_in = nullptr;
    void *host_out = nullptr;


    explicit not_gate_sim_chip(const api::plugin_api_t &plugin)
        : not_gate_sim_chip(plugin, {0, 0, 0}, {0, 0, 0, new bool}) {
    }

private:
    not_gate_sim_chip(const api::plugin_api_t &plugin,
                      const api::pin_sink_t &in,
                      const api::pin_t &out)
        : simulation_chip_impl_t(3, 3),
          in{in},
          out{out},
          host_in(plugin.create_simulation_sink(in)),
          host_out(plugin.create_simulation_pin(out)) {
    }

public:
    void update(const std::function<void(void *pin)> pin_updated_fn) override {
        if (*static_cast<bool *>(in.target->value) != *static_cast<bool *>(out.value)) {
            *static_cast<bool *>(out.value) = !*static_cast<bool *>(in.target->value);
            pin_updated_fn(host_out);
        }
    }

    const api::pin_t *get_pins(size_t *count) const override {
        *count = 1;
        return &out;
    }

    const api::pin_sink_t *get_sinks(size_t *count) const override {
        *count = 1;
        return &in;
    }
};

class default_plugin_t;

struct not_gate_reg_chip final : register_chip_impl_t {
    const api::plugin_api_t *plugin_api;

    explicit not_gate_reg_chip(const api::plugin_api_t *plugin_api)
        : register_chip_impl_t("de.codelix:and"), plugin_api(plugin_api) {
    }

    simulation_chip_impl_t *create_simulation_chip() override {
        return new not_gate_sim_chip(*plugin_api);
    }

};

class default_plugin_t final : public plugin_impl_t {
public:
    default_plugin_t() : plugin_impl_t("de.codelix:default", 0) {
    }

    void init(const api::plugin_api_t *plugin_api) override {
        std::cout << "Hello World! From Plugin " << this->name << std::endl;
        std::cout << "Plugin API Version: " << plugin_api->version << std::endl;

        const auto reg_chip = new not_gate_reg_chip(plugin_api);
        plugin_api->register_chip(*reg_chip);
    }

    void shutdown(const api::plugin_api_t *plugin_api) override {
        std::cout << "Bye World! From Plugin " << this->name << std::endl;
    }
};


#ifdef __cplusplus
extern "C" {
#endif

api::plugin_t *create_plugin() {
    auto *plugin = new default_plugin_t{};
    return plugin->handle();
};


#ifdef __cplusplus
}
#endif