//
// Created by felix on 27.04.25.
//

#include "controller/pinConnector.h"
#include "model/pinLookupCache.h"

using namespace Controller;

flecs::observer &pinConnector(const flecs::world &world, const Models::PinLookupCache &pinCache) {
    return world.observer<Models::Joint, Models::Position>()
        .yield_existing()
        .event(flecs::OnAdd)
        .event(flecs::OnSet)
        .event(flecs::OnRemove)
        .each([pinCache](const flecs::iter& it, size_t, const Models::Joint&, const Models::Position& pos) {
            Models::Pin* pin = pinCache.getPin(pos);
            // Check Network Component
    });
}

flecs::observer &pinDisconnector(const flecs::world &world, const Models::PinLookupCache &pinCache) {

}