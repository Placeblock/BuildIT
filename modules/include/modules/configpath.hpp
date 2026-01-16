//
// Created by felix on 1/16/26.
//

#ifndef BUILDIT_CONFIGPATH_HPP
#define BUILDIT_CONFIGPATH_HPP

#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <string>

#if defined(__linux__) || defined(BSD)
#define CFGPATH_LINUX
#elif defined(WIN32)
#define CFGPATH_WINDOWS
#elif defined(__APPLE__)
#define CFGPATH_MAC
#else
#error cfgpath.h functions have not been implemented for your platform!  Please send patches.
#endif

static std::filesystem::path get_home_config_path() {

#ifdef CFGPATH_LINUX
    char *config_home = std::getenv("XDG_CONFIG_HOME");
    if (!config_home) {
        char *home = std::getenv("HOME");
        if (!home) {
            throw std::runtime_error(
                "cannot find home directory. please make sure either HOME or XDG_CONFIG_HOME is set.");
        }
        std::filesystem::path config_path{home};
        config_path = config_path / ".config" / "BuildIT";
        return config_path;
    }
    std::filesystem::path config_path{config_home};
    config_path = config_path / "BuildIT";
    return config_path;
#elif CFGPATH_WINDOWS

#elif CFGPATH_MAC

#endif
}

#endif //BUILDIT_CONFIGPATH_HPP