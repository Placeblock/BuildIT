//
// Created by felix on 21.10.25.
//

#include "buildit/plugin-api.hpp"

simulation_chip_impl_t::simulation_chip_impl_t(const uint8_t width, const uint8_t height)
    : api::simulation_chip_t{width, height, &simulation_chip_impl_t::GetPins,
                             &simulation_chip_impl_t::GetSinks,
                             &simulation_chip_impl_t::Update, nullptr,
                             &simulation_chip_impl_t::Destroy} {
}

api::simulation_chip_t *simulation_chip_impl_t::handle() {
    return this;
}

void simulation_chip_impl_t::Update(api::simulation_chip_t *Self,
                                    const void *host_data,
                                    const api::pin_updated_fn_t pin_updated_fn) {
    static_cast<simulation_chip_impl_t *>(Self)->update(
        [host_data,pin_updated_fn](const void *pin) {
            pin_updated_fn(host_data, pin);
        });
}

const api::pin_t *simulation_chip_impl_t::GetPins(const api::simulation_chip_t *Self,
                                                  size_t *count) {
    return static_cast<const simulation_chip_impl_t *>(Self)->get_pins(count);
}

const api::pin_sink_t *simulation_chip_impl_t::GetSinks(const api::simulation_chip_t *Self,
                                                        size_t *count) {
    return static_cast<const simulation_chip_impl_t *>(Self)->get_sinks(count);
}

void simulation_chip_impl_t::Destroy(const api::simulation_chip_t *Self) {
    delete static_cast<const simulation_chip_impl_t *>(Self);
}

register_chip_impl_t::register_chip_impl_t(const char *name)
    : api::register_chip_t(name,
                           &register_chip_impl_t::CreateSimulationChip,
                           &register_chip_impl_t::Destroy) {
}

api::register_chip_t *register_chip_impl_t::handle() {
    return this;
}

api::simulation_chip_t *register_chip_impl_t::CreateSimulationChip(api::register_chip_t *Self) {
    return static_cast<register_chip_impl_t *>(Self)->create_simulation_chip();
}

void register_chip_impl_t::Destroy(const api::register_chip_t *Self) {
    delete static_cast<const register_chip_impl_t *>(Self);
}

plugin_impl_t::plugin_impl_t(const std::string &name, uint16_t version)
    : api::plugin_t{name.c_str(), version, &plugin_impl_t::Init,
                    &plugin_impl_t::Shutdown,
                    &plugin_impl_t::Destroy} {
}

api::plugin_t *plugin_impl_t::handle() {
    return this;
}

void plugin_impl_t::Init(api::plugin_t *Self, const api::plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->init(plugin_api);
}

void plugin_impl_t::Shutdown(api::plugin_t *Self, const api::plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->shutdown(plugin_api);
}

void plugin_impl_t::Destroy(const api::plugin_t *Self) {
    delete static_cast<const plugin_impl_t *>(Self);
}