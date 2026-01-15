//
// Created by felix on 21.10.25.
//

#ifndef BUILDIT_PLUGIN_API_CPP_H
#define BUILDIT_PLUGIN_API_CPP_H

#include "buildit/plugin-api.h"
#include <functional>
#include <string>

namespace buildit::api {
class simulation_node_impl_t : public simulation_node_t {
public:
    explicit simulation_node_impl_t();

    virtual ~simulation_node_impl_t() = default;

    virtual void update(std::function<void(void *pin)> pin_updated_fn) = 0;

    virtual pin_t *get_pins(size_t *count) = 0;

    virtual pin_sink_t *get_sinks(size_t *count) = 0;

    simulation_node_t *handle();

private:
    static void Update(simulation_node_t *Self,
                       const void *host_data,
                       pin_updated_fn_t pin_updated_fn);

    static pin_t *GetPins(simulation_node_t *Self,
                          size_t *count);

    static pin_sink_t *GetSinks(simulation_node_t *Self,
                                size_t *count);

    static void Destroy(const simulation_node_t *Self);
};

class simulation_node_type_impl_t : public simulation_node_type_t {
public:
    explicit simulation_node_type_impl_t();

    virtual ~simulation_node_type_impl_t() = default;

    [[nodiscard]] virtual simulation_node_impl_t *create_node() const = 0;

    simulation_node_type_t *handle();

private:
    static simulation_node_t *CreateNode(const simulation_node_type_t *Self);

    static void Destroy(const simulation_node_type_t *Self);
};


class plugin_impl_t : public plugin_t {
public:
    const std::string name;

    explicit plugin_impl_t(const std::string &name, uint16_t version);

    virtual ~plugin_impl_t() = default;

    virtual void init(plugin_api_t *plugin_api) = 0;

    virtual void shutdown(plugin_api_t *plugin_api) = 0;

    plugin_t *handle();

private:
    static void Init(plugin_t *Self, plugin_api_t *plugin_api);

    static void Shutdown(plugin_t *Self, plugin_api_t *plugin_api);

    static void Destroy(const plugin_t *Self);
};
}

#endif //BUILDIT_PLUGIN_API_CPP_H