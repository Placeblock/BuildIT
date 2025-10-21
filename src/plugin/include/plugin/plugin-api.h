//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <cstdint>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
namespace api {
#endif

typedef struct pin_t {
    const uint16_t type;
    const uint8_t dx, dy;
    void *value;
} pin_t;

typedef struct pin_sink_t {
    const uint16_t type;
    const uint8_t dx, dy;
    pin_t *target;
} pin_sink_t;

typedef void (*pin_updated_fn_t)(const pin_t *pin);

typedef struct simulation_chip_t {
    const uint8_t width, height;

    const pin_t *(*get_pins)(const simulation_chip_t *chip, size_t *count);

    const pin_sink_t *(*get_sinks)(const simulation_chip_t *chip, size_t *count);

    void (*update)(simulation_chip_t *chip, pin_updated_fn_t pin_updated_fn);

    void *(*update_graphics_component)(const simulation_chip_t *chip);

    void (*destroy)(const simulation_chip_t *chip);
} simulation_chip_t;

typedef struct register_chip_t {
    const char *name;

    simulation_chip_t *(*create_simulation_chip)();
} register_chip_t;

typedef struct plugin_api_t {
    int version;

    void (*register_chip)(register_chip_t &register_chip);

    void *(*get_graphics_components)(size_t *count);
} plugin_api_t;

typedef struct plugin_t {
    const char *name;
    int version;

    void (*init)(const plugin_t *plugin, const plugin_api_t *plugin_api);

    void (*shutdown)(const plugin_t *plugin, const plugin_api_t *plugin_api);

    void (*destroy)(const plugin_t *plugin);
} plugin_t;

typedef plugin_t * (*create_plugin_fn_t)();

#ifdef __cplusplus
}
}
#endif

#endif //PLUGIN_HPP
