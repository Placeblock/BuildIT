//
// Created by felix on 21.10.25.
//

#include "buildit/plugin-api.hpp"

#include <iostream>
#include <ostream>

chip_impl_t::chip_impl_t()
    : api::chip_t{&chip_impl_t::GetPins,
                  &chip_impl_t::GetSinks,
                  &chip_impl_t::Update,
                  &chip_impl_t::Destroy} {
}

api::chip_t *chip_impl_t::handle() {
    return this;
}

void chip_impl_t::Update(api::chip_t *Self,
                         const void *host_data,
                         const api::pin_updated_fn_t pin_updated_fn) {
    static_cast<chip_impl_t *>(Self)->update(
        [host_data,pin_updated_fn](const void *pin) {
            pin_updated_fn(host_data, pin);
        });
}

api::pin_t *chip_impl_t::GetPins(api::chip_t *Self,
                                 size_t *count) {
    return static_cast<chip_impl_t *>(Self)->get_pins(count);
}

api::pin_sink_t *chip_impl_t::GetSinks(api::chip_t *Self,
                                       size_t *count) {
    return static_cast<chip_impl_t *>(Self)->get_sinks(count);
}

void chip_impl_t::Destroy(const api::chip_t *Self) {
    delete static_cast<const chip_impl_t *>(Self);
}

chip_type_impl_t::chip_type_impl_t(const char *name, uint8_t width, uint8_t height)
    : api::chip_type_t(name,
                       width,
                       height,
                       &chip_type_impl_t::CreateChip,
                       &chip_type_impl_t::Destroy) {
}

api::chip_type_t *chip_type_impl_t::handle() {
    return this;
}

api::chip_t *chip_type_impl_t::CreateChip(api::chip_type_t *Self) {
    return static_cast<chip_type_impl_t *>(Self)->create_chip();
}

void chip_type_impl_t::Destroy(const api::chip_type_t *Self) {
    delete static_cast<const chip_type_impl_t *>(Self);
}

plugin_impl_t::plugin_impl_t(const std::string &name, uint16_t version)
    : api::plugin_t{name.c_str(), version, &plugin_impl_t::Init,
                    &plugin_impl_t::Shutdown,
                    &plugin_impl_t::Destroy}, name(name) {
    api::plugin_t::name = this->name.c_str();
    std::cout << api::plugin_t::name << std::endl;
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