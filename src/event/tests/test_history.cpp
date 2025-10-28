//
// Created by felix on 08.05.25.
//

#include "event/listener/create_chip_event.h"
#include <entt/entt.hpp>

int main() {
    entt::registry registry;
    entt::dispatcher dispatcher;

    create_chip_event_handler_t handler{};

    dispatcher.sink<create_chip_event_t>().connect<&create_chip_event_handler_t::on>(handler);

}
