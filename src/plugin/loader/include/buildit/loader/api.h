//
// Created by felix on 28.10.25.
//

#ifndef BUILDIT_API_H
#define BUILDIT_API_H
#include "buildit/plugin-api.h"
#include "simulation/node.hpp"

namespace buildit::loader {

struct plugin_sim_node_t final : sim::node_t {
    explicit plugin_sim_node_t(api::chip_t *handle);

    void update(const std::function<void(const sim::pin_t *pin)> &on_updated) override;

    [[nodiscard]] std::vector<sim::pin_t> get_pins() const override;

    [[nodiscard]] std::vector<sim::pin_sink_t> get_pin_sinks() const override;

    std::vector<sim::pin_t> pins;
    std::vector<sim::pin_sink_t> sinks;

private:
    api::chip_t *handle;
};

struct plugin_api_impl_t : api::plugin_api_t {
    explicit plugin_api_impl_t(int version);

    virtual ~plugin_api_impl_t() = default;

    virtual void register_chip_type(api::chip_type_t *chip_type) = 0;

    virtual void *get_graphics_component(size_t *count) = 0;

private:
    static void RegisterChipType(api::plugin_api_t *plugin_api, api::chip_type_t *chip_type);

    static void *GetGraphicsComponent(api::plugin_api_t *plugin_api, size_t *count);
};

}

#endif //BUILDIT_API_H