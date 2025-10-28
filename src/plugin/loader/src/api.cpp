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