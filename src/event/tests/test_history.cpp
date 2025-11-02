//
// Created by felix on 08.05.25.
//

#include "event/listener/create_chip_event.h"
#include <entt/entt.hpp>

using namespace buildit::event;

int main() {
    buildit::ecs::chip_type_registry_t chip_type_registry;
    entt::registry registry;
    entt::dispatcher dispatcher;

    create_chip_event_handler_t handler{chip_type_registry, registry};

    dispatcher.sink<create_chip_event_t>().connect<&create_chip_event_handler_t::on>(handler);

    dispatcher.enqueue<create_chip_event_t>(nullptr, "de.codelix:test", glm::vec2{0, 0});

    dispatcher.update();
}
