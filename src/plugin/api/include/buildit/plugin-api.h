//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace buildit::api {
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

typedef struct simulation_node_t {
    pin_t *(*get_pins)(simulation_node_t *chip, size_t *count);

    pin_sink_t *(*get_sinks)(simulation_node_t *chip, size_t *count);

    void (*update)(simulation_node_t *chip, const void *host_data, pin_updated_fn_t pin_updated_fn);

    void (*destroy)(const simulation_node_t *chip);
} simulation_node_t;

typedef struct simulation_node_type_t {
    const char *name;

    simulation_node_t *(*create_node)(const simulation_node_type_t *simulation_node_type);

    void (*destroy)(const simulation_node_type_t *chip);
} simulation_node_type_t;

typedef struct graphics_chip_t {
    uint8_t width, height;

    void (*destroy)(const graphics_chip_t *chip);
} graphics_chip_t;

typedef struct graphics_chip_type_t {
    const char *name;

    // TODO: Serialization and deserialization

    void *(*get_graphics_components)(graphics_chip_type_t *chip_type, size_t *count);

    void (*destroy)(const graphics_chip_type_t *chip);
} graphics_chip_type_t;

typedef struct simulation_to_graphics_converter_t {
    const char *name;

    graphics_chip_t *(*convert)(const simulation_node_t *simulation_node);
} simulation_to_graphics_converter_t;

typedef struct plugin_api_t {
    int version;

    void (*register_simulation_node_type)(plugin_api_t *plugin_api,
                                          simulation_node_type_t *simulation_node_type);

    void (*register_graphics_chip_type)(plugin_api_t *plugin_api,
                                        graphics_chip_type_t *graphics_chip_type);

    void (*register_converter)(plugin_api_t *plugin_api,
                               simulation_to_graphics_converter_t *converter);
} plugin_api_t;

typedef struct plugin_t {
    const char *name;
    int version;

    void (*init)(plugin_t *plugin, plugin_api_t *plugin_api);

    void (*shutdown)(plugin_t *plugin, plugin_api_t *plugin_api);

    void (*destroy)(const plugin_t *plugin);
} plugin_t;

typedef plugin_t * (*create_plugin_fn_t)();

#ifdef __cplusplus
}
}
#endif

#endif //PLUGIN_HPP
