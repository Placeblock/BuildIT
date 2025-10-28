//
// Created by felix on 28.10.25.
//

#include "buildit/loader/api.h"

buildit::loader::plugin_sim_node_t::plugin_sim_node_t(api::chip_t *handle) : handle(handle) {

}

void buildit::loader::plugin_sim_node_t::update(
    const std::function<void(const sim::pin_t *pin)> &on_updated) {
    this->handle->update(this->handle,
                         &on_updated,
                         [](const void *host_data, const void *sim_pin_impl) {
                             (*static_cast<const std::function<void(const sim::pin_t *pin)> *>(
                                 host_data))(static_cast<const sim::pin_t *>(sim_pin_impl));
                         });
}

std::vector<sim::pin_t> buildit::loader::plugin_sim_node_t::get_pins() const {
    return this->pins;
}

std::vector<sim::pin_sink_t> buildit::loader::plugin_sim_node_t::get_pin_sinks() const {
    return this->sinks;
}

buildit::loader::plugin_api_impl_t::plugin_api_impl_t(int version)
    : plugin_api_t(version,
                   &plugin_api_impl_t::RegisterChipType,
                   &plugin_api_impl_t::GetGraphicsComponent) {
}

void buildit::loader::plugin_api_impl_t::RegisterChipType(api::plugin_api_t *plugin_api,
                                                          api::chip_type_t *chip_type) {
    static_cast<plugin_api_impl_t *>(plugin_api)->register_chip_type(chip_type);
}

void *buildit::loader::plugin_api_impl_t::GetGraphicsComponent(api::plugin_api_t *plugin_api,
                                                               size_t *count) {
    return static_cast<plugin_api_impl_t *>(plugin_api)->get_graphics_component(count);
}

