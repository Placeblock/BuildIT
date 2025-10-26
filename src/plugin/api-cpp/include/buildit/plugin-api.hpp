//
// Created by felix on 21.10.25.
//

#ifndef BUILDIT_PLUGIN_API_CPP_H
#define BUILDIT_PLUGIN_API_CPP_H

#include "buildit/plugin-api.h"
#include <functional>
#include <string>

namespace buildit::api {
class chip_impl_t : public chip_t {
public:
    explicit chip_impl_t();

    virtual ~chip_impl_t() = default;

    virtual void update(std::function<void(void *pin)> pin_updated_fn) = 0;

    virtual pin_t *get_pins(size_t *count) = 0;

    virtual pin_sink_t *get_sinks(size_t *count) = 0;

    chip_t *handle();

private:
    static void Update(chip_t *Self,
                       const void *host_data,
                       pin_updated_fn_t pin_updated_fn);

    static pin_t *GetPins(chip_t *Self,
                          size_t *count);

    static pin_sink_t *GetSinks(chip_t *Self,
                                size_t *count);

    static void Destroy(const chip_t *Self);
};

class chip_type_impl_t : public chip_type_t {
public:
    explicit chip_type_impl_t(const char *name, uint8_t width, uint8_t height);

    virtual ~chip_type_impl_t() = default;

    virtual chip_impl_t *create_chip() = 0;

    chip_type_t *handle();

private:
    static chip_t *CreateChip(chip_type_t *Self);

    static void Destroy(const chip_type_t *Self);
};


class plugin_impl_t : public plugin_t {
public:
    const std::string name;

    explicit plugin_impl_t(const std::string &name, uint16_t version);

    virtual ~plugin_impl_t() = default;

    virtual void init(const plugin_api_t *plugin_api) = 0;

    virtual void shutdown(const plugin_api_t *plugin_api) = 0;

    plugin_t *handle();

private:
    static void Init(plugin_t *Self, const plugin_api_t *plugin_api);

    static void Shutdown(plugin_t *Self, const plugin_api_t *plugin_api);

    static void Destroy(const plugin_t *Self);
};
}

#endif //BUILDIT_PLUGIN_API_CPP_H