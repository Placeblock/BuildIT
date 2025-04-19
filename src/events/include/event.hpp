//
// Created by felix on 30.03.25.
//

#ifndef EVENT_H
#define EVENT_H
#include <cstdint>
#include <sstream>
#include <vector>
#include <entt/entity/snapshot.hpp>

#include "components.hpp"

namespace Events {
    struct Event {
        bool undo = false;
    };

    struct MoveEvent : Event {
        std::vector<uint32_t> entities;
        Models::Position delta;
    };

    struct RotateEvent : Event {
        std::vector<uint32_t> entities;
        Models::Rotation delta;
    };

    struct DeleteEvent : Event {
        uint32_t entity;
        std::stringstream snapshot;
    };

    struct CreateWireEvent : Event {
        std::tuple<uint32_t, uint32_t> joints;
    };
}



#endif //EVENT_H
