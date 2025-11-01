//
// Created by felix on 21.10.25.
//

#include "buildit/loader/loader.h"
#include "ecs/chip_type_registry.hpp"
#include <entt/entt.hpp>

using namespace buildit;

int main() {

    entt::registry registry;
    ecs::chip_type_registry_t chip_type_registry;
    loader::loader_t loader{registry, chip_type_registry};

    loader.load_plugins("./plugins");
    loader.init_plugins();

    return 0;
}