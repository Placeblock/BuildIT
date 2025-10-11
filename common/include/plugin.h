#ifndef PLUGIN_H
#define PLUGIN_H

#include "flecs/flecs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Plugin {
    const char* name;
    void (*init)(ecs_world_t* world);
    int (*do_work)(ecs_world_t* world);
    void (*shutdown)(ecs_world_t* world);
} Plugin;

// Plugin entry point
typedef Plugin* (*CreatePluginFn)(void);

// Symbol name plugins must export
#define CREATE_PLUGIN_SYMBOL "create_plugin"
#define VERSION "1"

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H