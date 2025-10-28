//
// Created by felix on 21.10.25.
//

#include "buildit/plugin-api.hpp"

#include <iostream>
#include <ostream>

using namespace buildit::api;

chip_impl_t::chip_impl_t()
    : chip_t{&chip_impl_t::GetPins,
             &chip_impl_t::GetSinks,
             &chip_impl_t::Update,
             &chip_impl_t::Destroy} {
}

chip_t *chip_impl_t::handle() {
    return this;
}

void chip_impl_t::Update(chip_t *Self,
                         const void *host_data,
                         const pin_updated_fn_t pin_updated_fn) {
    static_cast<chip_impl_t *>(Self)->update(
        [host_data,pin_updated_fn](const void *pin) {
            pin_updated_fn(host_data, pin);
        });
}

pin_t *chip_impl_t::GetPins(chip_t *Self,
                            size_t *count) {
    return static_cast<chip_impl_t *>(Self)->get_pins(count);
}

pin_sink_t *chip_impl_t::GetSinks(chip_t *Self,
                                  size_t *count) {
    return static_cast<chip_impl_t *>(Self)->get_sinks(count);
}

void chip_impl_t::Destroy(const chip_t *Self) {
    delete static_cast<const chip_impl_t *>(Self);
}

chip_type_impl_t::chip_type_impl_t(const char *name, uint8_t width, uint8_t height)
    : chip_type_t(name,
                  width,
                  height,
                  &chip_type_impl_t::CreateChip,
                  &chip_type_impl_t::Destroy) {
}

chip_type_t *chip_type_impl_t::handle() {
    return this;
}

chip_t *chip_type_impl_t::CreateChip(const chip_type_t *Self) {
    return static_cast<const chip_type_impl_t *>(Self)->create_chip();
}

void chip_type_impl_t::Destroy(const chip_type_t *Self) {
    delete static_cast<const chip_type_impl_t *>(Self);
}

plugin_impl_t::plugin_impl_t(const std::string &name, uint16_t version)
    : plugin_t{name.c_str(), version, &plugin_impl_t::Init,
               &plugin_impl_t::Shutdown,
               &plugin_impl_t::Destroy}, name(name) {
    plugin_t::name = this->name.c_str();
    std::cout << plugin_t::name << std::endl;
}

plugin_t *plugin_impl_t::handle() {
    return this;
}

void plugin_impl_t::Init(plugin_t *Self, const plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->init(plugin_api);
}

void plugin_impl_t::Shutdown(plugin_t *Self, const plugin_api_t *plugin_api) {
    static_cast<plugin_impl_t *>(Self)->shutdown(plugin_api);
}

void plugin_impl_t::Destroy(const plugin_t *Self) {
    delete static_cast<const plugin_impl_t *>(Self);
}