//
// Created by felix on 21.10.25.
//

#include "buildit/plugin-api.hpp"

#include <iostream>
#include <ostream>

using namespace buildit::api;

simulation_node_impl_t::simulation_node_impl_t()
    : simulation_node_t{&simulation_node_impl_t::GetPins,
                        &simulation_node_impl_t::GetSinks,
                        &simulation_node_impl_t::Update,
                        &simulation_node_impl_t::Destroy} {
}

simulation_node_t *simulation_node_impl_t::handle() {
    return this;
}

void simulation_node_impl_t::Update(simulation_node_t *Self,
                                    const void *host_data,
                                    const pin_updated_fn_t pin_updated_fn) {
    static_cast<simulation_node_impl_t *>(Self)->update(
        [host_data,pin_updated_fn](const void *pin) {
            pin_updated_fn(host_data, pin);
        });
}

pin_t *simulation_node_impl_t::GetPins(simulation_node_t *Self,
                                       size_t *count) {
    return static_cast<simulation_node_impl_t *>(Self)->get_pins(count);
}

pin_sink_t *simulation_node_impl_t::GetSinks(simulation_node_t *Self,
                                             size_t *count) {
    return static_cast<simulation_node_impl_t *>(Self)->get_sinks(count);
}

void simulation_node_impl_t::Destroy(const simulation_node_t *Self) {
    delete static_cast<const simulation_node_impl_t *>(Self);
}

simulation_node_type_impl_t::simulation_node_type_impl_t()
    : simulation_node_type_t(&simulation_node_type_impl_t::CreateNode,
                             &simulation_node_type_impl_t::Destroy) {
}

simulation_node_type_t *simulation_node_type_impl_t::handle() {
    return this;
}

simulation_node_t *simulation_node_type_impl_t::CreateNode(const simulation_node_type_t *Self) {
    return static_cast<const simulation_node_type_impl_t *>(Self)->create_node();
}

void simulation_node_type_impl_t::Destroy(const simulation_node_type_t *Self) {
    delete static_cast<const simulation_node_type_impl_t *>(Self);
}

plugin_impl_t::plugin_impl_t(const std::string &name, const uint16_t version)
    : plugin_t{name.c_str(), version, &plugin_impl_t::Init,
               &plugin_impl_t::Shutdown,
               &plugin_impl_t::Destroy}, name(name) {
    plugin_t::name = this->name.c_str();
    std::cout << plugin_t::name << std::endl;
}

plugin_t *plugin_impl_t::handle() {
    return this;
}

void plugin_impl_t::Init(plugin_t *Self, plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->init(plugin_api);
}

void plugin_impl_t::Shutdown(plugin_t *Self, plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->shutdown(plugin_api);
}

void plugin_impl_t::Destroy(const plugin_t *Self) {
    delete static_cast<const plugin_impl_t *>(Self);
}