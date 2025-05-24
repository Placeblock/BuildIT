//
// Created by felix on 11.05.25.
//

#include <string>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "CLI/CLI.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    CLI::App app{"BuildIT - CLI"};
    argv = app.ensure_utf8(argv);

    std::string rel_project_dir;
    const auto start_command = app.add_subcommand("start", "Starts and servers a project.");
    start_command->add_option("project", rel_project_dir, "The project directory containing the project to load")->required();

    CLI11_PARSE(app, argc, argv);

    if (*start_command) {
        const std::filesystem::path project_dir = fs::absolute(rel_project_dir);
        if (!fs::exists(project_dir)) {
            spdlog::error("Project directory {} does not exist", project_dir.string());
            return 1;
        }
        const std::filesystem::path plugins_dir = project_dir / "plugins";
        if (!fs::is_directory(plugins_dir)) {
            spdlog::error("Project directory {} does not contain a plugins directory", project_dir.string());
            return 1;
        }
        const std::filesystem::path macros_dir = project_dir / "macros";
        if (!fs::is_directory(plugins_dir)) {
            spdlog::error("Project directory {} does not contain a macros directory", project_dir.string());
            return 1;
        }
        const std::filesystem::path board_dir = project_dir / "board";
        if (!fs::is_directory(board_dir)) {
            spdlog::error("Project directory {} does not contain a board directory", project_dir.string());
            return 1;
        }
        
    } else {
        spdlog::info("No command specified.");
        std::cout << app.help() << std::endl;
    }

    return 1;
}