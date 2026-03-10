//
// Created by felix on 1/15/26.
//

#include <CLI/CLI.hpp>
#include "spdlog/spdlog.h"
#include <dlfcn.h>
#include <string>
#include "modules/module_api.hpp"
#include "environment.hpp"
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include "ecs_history/history.hpp"
#include "ecs_history/serialization/change.hpp"
#include "ecs_history/serialization/serialization.hpp"
#include "modules/configpath.hpp"
#include <filesystem>

using namespace buildit;
using namespace entt::literals;

std::unique_ptr<modules::api::module_t> load_module(const std::filesystem::path &path) {
    spdlog::info("loading Module " + path.string());
#if _WIN32
    void *module = LoadLibrary((path.string() + ".dll").c_str());
    const auto create_plugin_fn = reinterpret_cast<modules::api::create_module_fn_t>(GetProcAddress(
        module,
        "create_module"));
#else
    void *module = dlopen(path.string().c_str(), RTLD_LAZY);
    if (module == nullptr) {
        throw std::runtime_error("failed to load plugin " + path.string());
    }
    const auto create_plugin_fn = reinterpret_cast<modules::api::create_module_fn_t>(dlsym(
        module,
        "create_module"));
#endif
    if (!create_plugin_fn) {
        throw std::runtime_error("failed to load module " + path.string());
    }
    spdlog::info("loaded Module {}: {}", path.string(), module);
    return create_plugin_fn();
}

std::vector<std::unique_ptr<modules::api::module_t> > load_modules(
    const std::filesystem::path &modules_path) {
    std::vector<std::unique_ptr<modules::api::module_t> > modules;

    for (std::filesystem::directory_iterator dir_it(modules_path); const auto &file : dir_it) {
        if (!file.is_regular_file())
            continue;
        modules.push_back(load_module(file.path()));
    }

    return modules;
}


template<typename T>
void emplace(entt::registry &registry, const entt::entity entt, const T &value) {
    registry.emplace_or_replace<T>(entt, value);
}

int main(const int argc, char **argv) {
    ecs_history::serialization::initialize_component_serialization<
        cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive, bounding_box_t>();
    ecs_history::serialization::initialize_component_serialization<
        cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive, gate_t>();

    entt::registry reg;
    auto &entities = reg.ctx().emplace<ecs_history::static_entities_t>(reg);
    auto monitor = ecs_history::monitor_component<bounding_box_t>(reg);
    auto monitor2 = ecs_history::monitor_component<gate_t>(reg);
    std::vector<std::unique_ptr<ecs_history::base_storage_monitor_t> > monitors;
    monitors.push_back(std::move(monitor));
    monitors.push_back(std::move(monitor2));
    auto history = std::make_unique<ecs_history::history_t>(reg, monitors);

    modules::api::locked_registry_t locked_reg{reg, entities, monitors, *history};

    CLI::App app{"Logicgate simulation game", "BuildIT"};
    argv = app.ensure_utf8(argv);
    CLI11_PARSE(app, argc, argv);

    std::filesystem::path config_path = get_home_config_path();
    if (char *val = std::getenv("CONFIG_DIR"); val) {
        config_path = std::string(val);
    }

    std::filesystem::path module_path = MODULE_DIR;
    if (char *val = std::getenv("MODULE_DIR"); val) {
        module_path = std::string(val);
    }
    const std::vector<std::unique_ptr<modules::api::module_t> > modules = load_modules(module_path);

    const std::filesystem::path default_config_path{DEFAULT_CONFIG_DIR};

    for (const auto &module : modules) {
        spdlog::info("initializing module {}", module->get_name());
        std::filesystem::path module_config = config_path / (module->get_name() + ".ini");
        if (std::filesystem::is_regular_file(module_config)) {
            spdlog::info("found config file for module at " + module_config.string());
        } else {
            module_config = default_config_path / (module->get_name() + ".ini");
            spdlog::info("using default config for module at " + module_config.string());
        }

        ini::IniFile config;
        try {
            config = ini::IniFile(module_config);
        } catch (std::runtime_error &_) {
            throw std::runtime_error(
                "could not find config file for module " + module->get_name() + " at " + config_path
                .string());
        }
        module->init(config);
        module->run(locked_reg);

        spdlog::info("initialized module {}", module->get_name());
    }

    std::this_thread::sleep_until(std::chrono::steady_clock::time_point::max());

}