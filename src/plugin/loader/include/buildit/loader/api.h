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
    explicit plugin_sim_node_t(api::chip_t *handle);

    void update(const std::function<void(const sim::base_pin_t *pin)> &on_updated) override;

    [[nodiscard]] std::vector<sim::base_pin_t *> get_pins() override;

    [[nodiscard]] std::vector<sim::base_pin_sink_t *> get_pin_sinks() override;

    std::vector<plugin_sim_pin_t> pins;
    std::vector<plugin_sim_pin_sink_t> sinks;

private:
    api::chip_t *handle;
};

class plugin_api_impl_t : public api::plugin_api_t {
public:
    explicit plugin_api_impl_t(int version);

    virtual ~plugin_api_impl_t() = default;

private:
    virtual void register_chip_type(api::chip_type_t *chip_type) = 0;

    virtual void *get_graphics_components(size_t *count) = 0;

    static void RegisterChipType(api::plugin_api_t *plugin_api, api::chip_type_t *chip_type);

    static void *GetGraphicsComponents(api::plugin_api_t *plugin_api, size_t *count);
};

}

#endif //BUILDIT_API_H