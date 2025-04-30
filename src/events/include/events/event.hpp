//
// Created by felix on 30.03.25.
//

#ifndef EVENT_H
#define EVENT_H
#include <sstream>
#include <vector>

#include "model/components.hpp"

namespace Events {
    struct Event {
        bool undo = false;
    };

    struct WorldEvent : Event {
        uint16_t worldId;
    };

    struct MoveEvent : WorldEvent {
        std::vector<flecs::entity> entities;
        Models::Position delta;
    };

    struct RotateEvent : WorldEvent {
        std::vector<flecs::entity> entities;
        Models::Rotation delta;
    };

    struct DeleteEvent : WorldEvent {
        std::vector<flecs::entity> entities;
        std::stringstream snapshot;
    };

    struct CreateWireEvent : WorldEvent {
        std::tuple<flecs::entity, flecs::entity> joints;
    };
}



#endif //EVENT_H
