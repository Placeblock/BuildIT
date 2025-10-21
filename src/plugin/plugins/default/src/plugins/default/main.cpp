//
// Created by felix on 21.10.25.
//


#include "plugin/plugin-api.h"
#include <iostream>

class sim_chip_t : api::simulation_chip_t {
public:
    explicit sim_chip_t(uint8_t width, uint8_t height);

    virtual ~sim_chip_t() = default;

    virtual void update(api::pin_updated_fn_t pin_updated_fn) = 0;

    virtual const api::pin_t *get_pins(size_t *count) const = 0;

    virtual const api::pin_sink_t *get_sinks(size_t *count) const = 0;

    api::simulation_chip_t *handle() {
        return this;
    }

private:
    static void Update(api::simulation_chip_t *Self,
                       const api::pin_updated_fn_t pin_updated_fn) {
        static_cast<sim_chip_t *>(Self)->update(pin_updated_fn);
    }

    static const api::pin_t *GetPins(const api::simulation_chip_t *Self,
                                     size_t *count) {
        return static_cast<const sim_chip_t *>(Self)->get_pins(count);
    }

    static const api::pin_sink_t *GetSinks(const api::simulation_chip_t *Self,
                                           size_t *count) {
        return static_cast<const sim_chip_t *>(Self)->get_sinks(count);
    }

    static void Destroy(const api::simulation_chip_t *Self) {
        delete static_cast<const sim_chip_t *>(Self);
    }
};

sim_chip_t::sim_chip_t(const uint8_t width, const uint8_t height)
    : api::simulation_chip_t{width, height, &sim_chip_t::GetPins, &sim_chip_t::GetSinks,
                             &sim_chip_t::Update, nullptr,
                             &sim_chip_t::Destroy} {
}


#ifdef __cplusplus
extern "C" {
#endif

struct not_gate_sim_chip final : sim_chip_t {
    api::pin_sink_t in;
    api::pin_t out;

    not_gate_sim_chip() : sim_chip_t(3, 3),
                          in{0, 0, 0, nullptr},
                          out{0, 0, 0, new bool} {

    }

    void update(const api::pin_updated_fn_t pin_updated_fn) override {
        if (*static_cast<bool *>(in.target->value) != *static_cast<bool *>(out.value)) {
            *static_cast<bool *>(out.value) = !*static_cast<bool *>(in.target->value);
            pin_updated_fn(&out);
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

api::simulation_chip_t *create_simulation_chip() {
    const auto chip = new not_gate_sim_chip();
    return chip->handle();
}

api::plugin_t *create_plugin() {
    auto *plugin = new api::plugin_t();

    plugin->name = "de.codelix:default";
    plugin->version = 0;
    plugin->init = [](const api::plugin_t *plugin, const api::plugin_api_t *plugin_api) {
        std::cout << "Hello World! From Plugin " << plugin->name << std::endl;
        std::cout << "Plugin API Version: " << plugin_api->version << std::endl;

        api::register_chip_t register_chip{
            "de.codelix:and"
        };
        register_chip.create_simulation_chip = [] {
            return create_simulation_chip();
        };
        plugin_api->register_chip(register_chip);
    };
    plugin->shutdown = [](const api::plugin_t *plugin, const api::plugin_api_t *plugin_api) {
        std::cout << "Bye World! From Plugin " << plugin->name << std::endl;
    };
    plugin->destroy = [](const api::plugin_t *to_delete_plugin) {
        delete to_delete_plugin;
    };

    return plugin;
};


#ifdef __cplusplus
}
#endif