//
// Created by felix on 28.10.25.
//

#ifndef BUILDIT_API_H
#define BUILDIT_API_H
#include "buildit/plugin-api.h"
#include "simulation/node.hpp"

namespace buildit::loader {

struct plugin_sim_pin_t final : sim::base_pin_t {
    void *value;

    explicit plugin_sim_pin_t(const uint16_t type, void *value) : base_pin_t(type), value(value) {
    }

    void *get_value_ptr() override {
        return value;
    }
};

struct plugin_sim_pin_sink_t final : sim::base_pin_sink_t {
    explicit plugin_sim_pin_sink_t(api::pin_sink_t *handle) : base_pin_sink_t(handle->type),
                                                              handle(handle) {
    }

    void **get_pin_value() override {
        return handle->pin_value;
    }

    api::pin_sink_t *handle;
};

struct plugin_sim_node_t final : sim::node_t {
    explicit plugin_sim_node_t(api::simulation_node_t *handle);

    void update(const std::function<void(const sim::base_pin_t *pin)> &on_updated) override;

    [[nodiscard]] std::vector<sim::base_pin_t *> get_pins() override;

    [[nodiscard]] std::vector<sim::base_pin_sink_t *> get_pin_sinks() override;

    std::vector<plugin_sim_pin_t> pins;
    std::vector<plugin_sim_pin_sink_t> sinks;

private:
    api::simulation_node_t *handle;
};

class plugin_api_impl_t : public api::plugin_api_t {
public:
    explicit plugin_api_impl_t(int version);

    virtual ~plugin_api_impl_t() = default;

private:
    virtual void register_simulation_node_type(const char *chip_type_name,
                                               api::simulation_node_type_t *node_type) = 0;

    virtual void register_graphics_chip_type(const char *chip_type_name,
                                             api::graphics_chip_type_t *chip_type) = 0;

    virtual void register_converter(const char *chip_type_name,
                                    api::simulation_to_graphics_converter_t *converter) = 0;

    static void RegisterSimulationNodeType(api::plugin_api_t *plugin_api,
                                           api::simulation_node_type_t *chip_type);
};

}

#endif //BUILDIT_API_H