//
// Created by felix on 21.10.25.
//

#include "plugin-api/plugin-api.h"
#include <dlfcn.h>
#include <iostream>
#include <ostream>

int main() {
    api::plugin_api_t api{0};
    api.register_chip = [](api::register_chip_t &chip) {
        std::cout << "Registering chip " << chip.name << std::endl;
    };

    void *test_plugin = dlopen("../plugins/default/libplugin_default.so",
                               RTLD_NOW | RTLD_GLOBAL);
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(dlsym(
        test_plugin,
        "create_plugin"));
    const api::plugin_t *plugin = create_plugin_fn();
    std::cout << "Loaded plugin " << plugin->name << " with version " << plugin->version <<
        std::endl;
    plugin->init(plugin, &api);
    plugin->shutdown(plugin, &api);
    plugin->destroy(plugin);
}