//
// Created by felix on 30.03.25.
//

#ifndef EVENT_H
#define EVENT_H
#include <sstream>
#include <vector>

#include "components.hpp"
#include "flecs/addons/cpp/entity.hpp"
#include "model/components.hpp"

namespace Events {
    struct Event {
        bool undo = false;
    };

    struct MoveEvent : Event {
        std::vector<flecs::entity> entities;
        Models::Position delta;
    };

    struct RotateEvent : Event {
        std::vector<flecs::entity> entities;
        Models::Rotation delta;
    };

    struct DeleteEvent : Event {
        std::vector<flecs::entity> entities;
        std::stringstream snapshot;
    };

    struct CreateWireEvent : Event {
        std::tuple<flecs::entity, flecs::entity> joints;
    };
}



#endif //EVENT_H
