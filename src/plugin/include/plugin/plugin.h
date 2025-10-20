//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <cstdint>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pin_t {
    const uint16_t type;
    const uint8_t dx, dy;
    void *value;
} pin_t;

typedef struct pin_sink_t {
    const uint16_t type;
    const uint8_t dx, dy;
} pin_sink_t;

typedef void (*pin_updated_fn_t)();

typedef struct simulation_chip_t {
    const uint8_t width, height;
    const size_t pin_count;
    const pin_t *pins;
    const size_t sink_count;
    const pin_sink_t *sinks;

    const void (*update)(pin_updated_fn_t pin_updated_fn);

    const void *(*update_graphics_component)();

    const void (*destroy)();
} simulation_chip_t;

typedef struct register_chip_t {
    const char *name;

    const simulation_chip_t *(*create_simulation_chip)();

    const void (*destroy)();
} register_chip_t;

typedef struct plugin_api_t {
    const void (*register_chip)(const register_chip_t *register_chip);

    const void *(*get_graphics_components)(size_t *count);
} plugin_api_t;

typedef struct plugin_t {
    const char *name;

    const void (*init)(const plugin_api_t &plugin_api);

    const int (*do_work)(const plugin_api_t &plugin_api);

    const void (*shutdown)(const plugin_api_t &plugin_api);

    const void (*destroy)();
} plugin_t;

typedef plugin_t * (*create_plugin_fn)();

#ifdef __cplusplus
}
#endif

#endif //PLUGIN_HPP
