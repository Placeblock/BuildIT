//
// Created by felix on 1/15/26.
//

#include <CLI/CLI.hpp>
#include "spdlog/spdlog.h"
#include <dlfcn.h>
#include <string>
#include "modules/module_api.hpp"
#include "environment.hpp"
#include <filesystem>

using namespace buildit;

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

int main(int argc, char **argv) {
    CLI::App app{"Logicgate simulation game", "BuildIT"};
    argv = app.ensure_utf8(argv);
    CLI11_PARSE(app, argc, argv);

    std::filesystem::path config_path = CONFIG_DIR;
    if (char *val = std::getenv("CONFIG_DIR"); val) {
        config_path = std::string(val);
    }

    std::filesystem::path module_path = MODULE_DIR;
    if (char *val = std::getenv("MODULE_DIR"); val) {
        module_path = std::string(val);
    }
    const std::vector<std::unique_ptr<modules::api::module_t> > modules = load_modules(module_path);

    for (const auto &module : modules) {
        spdlog::info("initializing module {}", module->get_name());

        try {
            ini::IniFile config(config_path / module->get_name());
            module->init(config);
        } catch (std::runtime_error &_) {
            throw std::runtime_error(
                "could not find config file for module " + module->get_name() + " in " + config_path
                .string());
        }

        spdlog::info("initialized module {}", module->get_name());
    }

    std::this_thread::sleep_until(std::chrono::steady_clock::time_point::max());

}