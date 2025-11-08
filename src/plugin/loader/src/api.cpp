//
// Created by felix on 28.10.25.
//

#include "buildit/loader/api.h"

using namespace buildit::loader;

plugin_sim_node_t::plugin_sim_node_t(api::simulation_node_t *handle) : handle(handle) {
    size_t pin_count = 0;
    const api::pin_t *api_pins = handle->get_pins(handle, &pin_count);
    for (size_t i = 0; i < pin_count; i++) {
        api::pin_t pin = api_pins[i];
        plugin_sim_pin_t sim_pin{pin.type, pin.value};
        this->pins.push_back(sim_pin);
        pin.sim_pin = &this->pins.back();
    }
    size_t pin_sink_count = 0;
    const api::pin_sink_t *api_pin_sinks = handle->get_sinks(handle, &pin_sink_count);
    for (size_t i = 0; i < pin_sink_count; i++) {
        api::pin_sink_t pin_sink = api_pin_sinks[i];
        plugin_sim_pin_sink_t sim_pin_sink{&pin_sink};
        this->sinks.push_back(sim_pin_sink);
    }
}

void plugin_sim_node_t::update(
    const std::function<void(const sim::base_pin_t *pin)> &on_updated) {
    this->handle->update(this->handle,
                         &on_updated,
                         [](const void *host_data, const void *sim_pin_impl) {
                             (*static_cast<const std::function<void(const sim::base_pin_t *pin)> *>(
                                 host_data))(static_cast<const sim::base_pin_t *>(sim_pin_impl));
                         });
}

std::vector<sim::base_pin_t *> plugin_sim_node_t::get_pins() {
    std::vector<sim::base_pin_t *> sim_pins;
    for (auto pin : this->pins) {
        sim_pins.push_back(&pin);
    }
    return sim_pins;
}

std::vector<sim::base_pin_sink_t *> plugin_sim_node_t::get_pin_sinks() {
    std::vector<sim::base_pin_sink_t *> sim_pin_sinks;
    for (auto pin_sink : this->sinks) {
        sim_pin_sinks.push_back(&pin_sink);
    }
    return sim_pin_sinks;
}

plugin_api_impl_t::plugin_api_impl_t(const int version)
    : plugin_api_t(version,
                   &plugin_api_impl_t::RegisterSimulationNodeType,
                   &plugin_api_impl_t::GetGraphicsComponents) {
}

void plugin_api_impl_t::RegisterSimulationNodeType(api::plugin_api_t *plugin_api,
                                                   api::simulation_node_type_t *chip_type) {
    static_cast<plugin_api_impl_t *>(plugin_api)->register_simulation_node_type(chip_type);
}

void *plugin_api_impl_t::GetGraphicsComponents(api::plugin_api_t *plugin_api,
                                               size_t *count) {
    return static_cast<plugin_api_impl_t *>(plugin_api)->get_graphics_components(count);
}

