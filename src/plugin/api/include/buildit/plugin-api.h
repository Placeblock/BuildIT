//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace api {
#endif

typedef struct pin_t {
    const uint16_t type;
    const uint8_t dx, dy;
    void *value;
    void *sim_pin;
} pin_t;

typedef struct pin_sink_t {
    const uint16_t type;
    const uint8_t dx, dy;
    void **pin_value;
} pin_sink_t;

typedef void (*pin_updated_fn_t)(const void *host_data, const void *sim_pin_impl);

typedef struct chip_t {
    pin_t *(*get_pins)(chip_t *chip, size_t *count);

    pin_sink_t *(*get_sinks)(chip_t *chip, size_t *count);

    void (*update)(chip_t *chip, const void *host_data, pin_updated_fn_t pin_updated_fn);

    void (*destroy)(const chip_t *chip);
} chip_t;

typedef struct chip_type_t {
    const char *name;
    const uint8_t width, height;

    chip_t *(*create_chip)(chip_type_t *chip_type);

    void (*destroy)(const chip_type_t *chip);
} chip_type_t;

typedef struct plugin_api_t {
    int version;

    void (*register_chip_type)(chip_type_t &chip_type);

    void *(*get_graphics_components)(size_t *count);
} plugin_api_t;

typedef struct plugin_t {
    const char *name;
    int version;

    void (*init)(plugin_t *plugin, const plugin_api_t *plugin_api);

    void (*shutdown)(plugin_t *plugin, const plugin_api_t *plugin_api);

    void (*destroy)(const plugin_t *plugin);
} plugin_t;

typedef plugin_t * (*create_plugin_fn_t)();

#ifdef __cplusplus
}
}
#endif

#endif //PLUGIN_HPP
