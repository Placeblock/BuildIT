//
// Created by felix on 21.10.25.
//


#include "plugin/plugin-api.h"
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

api::simulation_chip_t *create_simulation_chip() {
    const auto new_chip = new api::simulation_chip_t{
        3, 3, 1, nullptr, 1, nullptr,
        [](const api::simulation_chip_t *chip,
           const api::pin_updated_fn_t pin_updated_fn) {
            if (*static_cast<bool *>(chip->sinks[0].target->value) != *static_cast<bool *>(chip->
                    pins[0].value)) {
                *static_cast<bool *>(chip->pins[0].value) = !*static_cast<bool *>(chip->sinks[0].
                    target->value);
                pin_updated_fn(&chip->pins[0]);
            }
        }};
    return new_chip;
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