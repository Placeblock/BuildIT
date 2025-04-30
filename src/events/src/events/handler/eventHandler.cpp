//
// Created by felix on 28.04.25.
//

#include "events/handler/eventHandler.hpp"

using namespace Events;

void moveEventHandler(const MoveEvent &event) {
    for (const auto entity : event.entities) {
        event.world.entity(entity).set<Models::Move>({event.delta});
    }
}

void rotateEventHandler(const RotateEvent &event) {
    for (const auto entity : event.entities) {
        event.world.entity(entity).set<Models::Rotate>({event.delta});
    }
}
