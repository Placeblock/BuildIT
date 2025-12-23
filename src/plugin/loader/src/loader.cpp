//
// Created by felix on 28.10.25.
//

#include "buildit/loader/loader.h"

#include "spdlog/spdlog.h"
#include <dlfcn.h>
#include <filesystem>

using namespace buildit::loader;

loader_plugin_api_t::loader_plugin_api_t(entt::registry &reg,
                                         ecs::chip_type_registry_t &chip_type_registry)
    : plugin_api_impl_t(LOADER_VERSION), reg(reg), chip_type_registry(chip_type_registry) {
}

void *loader_plugin_api_t::get_graphics_components(size_t *count) {
    return nullptr;
}

void loader_plugin_api_t::register_simulation_node_type(const char *chip_type_name,
                                                        api::simulation_node_type_t *node_type) {
    std::unique_ptr<ecs::base_chip_type_t> ecs_chip_type = std::make_unique<
        ecs::plugin_chip_type_t>(chip_type);
    this->chip_type_registry.register_chip_type(chip_type_name, ecs_chip_type);
}

loader_t::loader_t(entt::registry &reg, ecs::chip_type_registry_t &chip_type_registry) : plugin_api(
    reg,
    chip_type_registry) {
}

void loader_t::load_plugins(const std::string &dir_path) {
    for (const auto &entry : std::filesystem::directory_iterator(dir_path)) {
        if (entry.is_regular_file()) {
            this->load_plugin(entry.path().string());
        }
    }
}

void loader_t::load_plugin(const std::string &file) {
    const auto plugin = load_plugin_file(file);
    this->plugins.push_back(plugin);
    spdlog::info("Loaded plugin {}", plugin->name);
}

void loader_t::init_plugins() {
    for (const auto plugin : this->plugins) {
        spdlog::info("Initializing plugin {}", plugin->name);
        plugin->init(plugin, &this->plugin_api);
        spdlog::info("Initialized plugin {}", plugin->name);
    }
}

buildit::api::plugin_t *loader_t::load_plugin_file(const std::string &file) {
#if _WIN32
    void *test_plugin = LoadLibrary((file + ".dll").c_str());
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(GetProcAddress(
        test_plugin,
        "create_plugin"));
#else
    void *test_plugin = dlopen(file.c_str(), RTLD_LAZY);
    if (test_plugin == nullptr) {
        spdlog::error("Failed to load plugin {}: {}", file, dlerror());
        throw std::runtime_error("Failed to load plugin");
    }
    const auto create_plugin_fn = reinterpret_cast<api::create_plugin_fn_t>(dlsym(
        test_plugin,
        "create_plugin"));
#endif
    return create_plugin_fn();
}

buildit::ecs::plugin_chip_type_t::plugin_chip_type_t(
    const api::simulation_node_type_t *handle) : chip_type_t(
                                                     handle->name,
                                                     handle->width,
                                                     handle->height), handle(handle) {
}

buildit::ecs::plugin_chip_type_t::plugin_chip_type_t(const plugin_chip_type_t &other) : chip_type_t(
        other.handle->name,
        other.handle->width,
        other.handle->height), handle(
        other.handle) {
}

plugin_sim_node_t buildit::ecs::plugin_chip_type_t::create_chip() const {
    api::simulation_node_t *chip_type = this->handle->create_chip(this->handle);
    return plugin_sim_node_t{chip_type};
}

buildit::ecs::plugin_graphics_component_t buildit::ecs::plugin_chip_type_t::get_graphics(
    simulation_type *sim) const {
    return {};
}