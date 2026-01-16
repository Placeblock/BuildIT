//
// Created by felix on 1/15/26.
//

#ifndef BUILDIT_MODULE_API_HPP
#define BUILDIT_MODULE_API_HPP
#include <memory>
#include "modules/inifile.hpp"

#ifdef _WIN32
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT
#endif

namespace buildit::modules::api {
class module_t {
public:
    [[nodiscard]] virtual std::string get_name() const = 0;

    [[nodiscard]] virtual std::string get_description() const = 0;

    virtual void init(ini::IniFile &config) = 0;

    virtual ~module_t() = default;
};

typedef std::unique_ptr<module_t> (*create_module_fn_t)();
}

#endif //BUILDIT_MODULE_API_HPP