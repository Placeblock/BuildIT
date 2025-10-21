//
// Created by felix on 21.10.25.
//

#ifndef BUILDIT_PLUGIN_API_CPP_H
#define BUILDIT_PLUGIN_API_CPP_H

#include "buildit/plugin-api.h"
#include <functional>
#include <string>

class simulation_chip_impl_t : public api::simulation_chip_t {
public:
    explicit simulation_chip_impl_t(uint8_t width, uint8_t height);

    virtual ~simulation_chip_impl_t() = default;

    virtual void update(std::function<void(void *pin)> pin_updated_fn) = 0;

    virtual const api::pin_t *get_pins(size_t *count) const = 0;

    virtual const api::pin_sink_t *get_sinks(size_t *count) const = 0;

    api::simulation_chip_t *handle();

private:
    static void Update(api::simulation_chip_t *Self,
                       const void *host_data,
                       api::pin_updated_fn_t pin_updated_fn);

    static const api::pin_t *GetPins(const api::simulation_chip_t *Self,
                                     size_t *count);

    static const api::pin_sink_t *GetSinks(const api::simulation_chip_t *Self,
                                           size_t *count);

    static void Destroy(const api::simulation_chip_t *Self);
};

class register_chip_impl_t : public api::register_chip_t {
public:
    explicit register_chip_impl_t(const char *name);

    virtual ~register_chip_impl_t() = default;

    virtual simulation_chip_impl_t *create_simulation_chip() = 0;

    api::register_chip_t *handle();

private:
    static api::simulation_chip_t *CreateSimulationChip(api::register_chip_t *Self);

    static void Destroy(const api::register_chip_t *Self);
};


class plugin_impl_t : public api::plugin_t {
public:
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