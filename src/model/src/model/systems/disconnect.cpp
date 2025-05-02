//
// Created by felix on 01.05.25.
//

#include "model/systems/disconnect.hpp""

#include "model/pinRenderCache.hpp"

flecs::system Systems::disconnect(const flecs::world &world) {
    return world.system<Models::Move>("Disconnect")
        .with<Models::Rotate>().or_()
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity e, const Models::Move *move, const Models::Rotate *rotate) {
            bool disconnect = (move != nullptr && move->disconnect) || (rotate != nullptr && rotate->disconnect);
            int32_t index = 0;
            flecs::entity child;
            while ((child = e.target(flecs::ChildOf, index++))) {
                if (const auto *pin = pinEntity.get<Models::Pin>(); pin != nullptr) {
                    pin.
                }
            }
        });
}
