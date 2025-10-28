//
// Created by felix on 28.10.25.
//

#include "buildit/loader/loader.h"

#include <dlfcn.h>

using namespace buildit::loader;

loader_plugin_api_t::loader_plugin_api_t(entt::registry &reg,
                                         ecs::chip_type_registry_t &chip_type_registry)
    : plugin_api_impl_t(LOADER_VERSION), reg(reg), chip_type_registry(chip_type_registry) {
}

void loader_plugin_api_t::register_chip_type(api::chip_type_t *chip_type) {
    std::unique_ptr<ecs::base_chip_type_t> ecs_chip_type = std::make_unique<
        ecs::plugin_chip_type_t>(chip_type);
    this->chip_type_registry.register_chip_type(chip_type->name, ecs_chip_type);
}

void *loader_plugin_api_t::get_graphics_components(size_t *count) {
    return nullptr;
}

loader_t::loader_t(entt::registry &reg, ecs::chip_type_registry_t &chip_type_registry) : plugin_api(
    reg,
    chip_type_registry) {
}

void loader_t::load_plugin(const std::string &file) {
    this->plugins.push_back(load_plugin_file(file));
}

void loader_t::init_plugins() const {
    for (const auto plugin : this->plugins) {
        plugin->init(plugin, &this->plugin_api);
    }
}

buildit::api::plugin_t *loader_t::load_plugin_file(const std::string &file) {
#if _WIN32
    void *test_plugin = LoadLibrary((file + ".dll").c_str());
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(GetProcAddress(
        test_plugin,
        "create_plugin"));
#else
    void *test_plugin = dlopen((file + ".so").c_str(), RTLD_LAZY);
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(dlsym(
        test_plugin,
        "create_plugin"));
#endif
    return create_plugin_fn();
}

buildit::ecs::plugin_chip_type_t::plugin_chip_type_t(const api::chip_type_t *handle) : chip_type_t(
        handle->name,
        handle->width,
        handle->height), handle(handle) {
}

plugin_sim_node_t buildit::ecs::plugin_chip_type_t::create_chip() const {
    api::chip_t *chip_type = this->handle->create_chip(this->handle);
    return plugin_sim_node_t(chip_type);
}

void *buildit::ecs::plugin_chip_type_t::get_graphics(simulation_type *sim) const {
    return nullptr;
}