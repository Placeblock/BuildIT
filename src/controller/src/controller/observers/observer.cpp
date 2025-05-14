//
// Created by felix on 02.05.25.
//

#include "controller/observers/observer.hpp"

#include "model/pinRenderCache.hpp"

flecs::observer updateSimulationLinks(const flecs::world &world) {
    return world.observer<>()
        .event(flecs::Monitor)
        .with<models::IsConnected>(flecs::Wildcard)
        .each([](const flecs::iter& it, const size_t i) {
            const flecs::entity e = it.entity(i);
            const flecs::entity target = it.pair(0).second();
            const models::Pin *pin = e.get<models::Pin>();
            const flecs::entity networkEntity = target.target<models::HasNetwork>();
            if (pin == nullptr || networkEntity.is_valid())
                return;
            models::Network *network = networkEntity.get_mut<models::Network>();
            if (network == nullptr)
                return;
            if (it.event() == flecs::OnRemove) {
                if (network->inputPin == )
            }
    });
}

flecs::observer synchronizeConnectedPair(const flecs::world &world) {
    return world.observer<>()
        .event(flecs::Monitor)
        .with<models::IsConnected>(flecs::Wildcard)
        .each([](const flecs::iter& it, const size_t i) {
            const flecs::entity e = it.entity(i);
            const flecs::entity target = it.pair(0).second();
            if (it.event() == flecs::OnAdd) {
                target.add<models::IsConnected>(e);
            } else if (it.event() == flecs::OnRemove) {
                target.remove<models::IsConnected>(e);
            }
        });
}
