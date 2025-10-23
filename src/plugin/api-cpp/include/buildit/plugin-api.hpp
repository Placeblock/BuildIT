//
// Created by felix on 21.10.25.
//

#ifndef BUILDIT_PLUGIN_API_CPP_H
#define BUILDIT_PLUGIN_API_CPP_H

#include "buildit/plugin-api.h"
#include <functional>
#include <string>

class chip_impl_t : public api::chip_t {
public:
    explicit chip_impl_t();

    virtual ~chip_impl_t() = default;

    virtual void update(std::function<void(void *pin)> pin_updated_fn) = 0;

    virtual api::pin_t *get_pins(size_t *count) = 0;

    virtual api::pin_sink_t *get_sinks(size_t *count) = 0;

    api::chip_t *handle();

private:
    static void Update(api::chip_t *Self,
                       const void *host_data,
                       api::pin_updated_fn_t pin_updated_fn);

    static api::pin_t *GetPins(api::chip_t *Self,
                               size_t *count);

    static api::pin_sink_t *GetSinks(api::chip_t *Self,
                                     size_t *count);

    static void Destroy(const api::chip_t *Self);
};

class chip_type_impl_t : public api::chip_type_t {
public:
    explicit chip_type_impl_t(const char *name, uint8_t width, uint8_t height);

    virtual ~chip_type_impl_t() = default;

    virtual chip_impl_t *create_chip() = 0;

    api::chip_type_t *handle();

private:
    static api::chip_t *CreateChip(api::chip_type_t *Self);

    static void Destroy(const api::chip_type_t *Self);
};


class plugin_impl_t : public api::plugin_t {
public:
    const std::string name;

    explicit plugin_impl_t(const std::string &name, uint16_t version);

    virtual ~plugin_impl_t() = default;

    virtual void init(const api::plugin_api_t *plugin_api) = 0;

    virtual void shutdown(const api::plugin_api_t *plugin_api) = 0;

    api::plugin_t *handle();

private:
    static void Init(api::plugin_t *Self, const api::plugin_api_t *plugin_api);

    static void Shutdown(api::plugin_t *Self, const api::plugin_api_t *plugin_api);

    static void Destroy(const api::plugin_t *Self);
};

#endif //BUILDIT_PLUGIN_API_CPP_H