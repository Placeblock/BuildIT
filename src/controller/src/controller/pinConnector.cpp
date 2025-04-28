//
// Created by felix on 27.04.25.
//

#include "controller/pinConnector.h"
#include "model/pinLookupCache.h"

using namespace Controller;




int main() {
    struct Position {
        int x;
        int y;
    };
    struct Bullet {};

    flecs::world world;
    flecs::entity entity = world.entity();
    entity.set<Position>({0, 0});

    world.system<Bullet, Position>().each([](Bullet& bullet, Position& pos) {

    });
}

flecs::observer &pinConnector(const flecs::world &world) {
    return world.observer<Models::Joint, Models::Position>()
        .yield_existing()
        .event(flecs::OnAdd)
        .event(flecs::OnSet)
        .event(flecs::OnRemove)
        .each([world](const flecs::iter& it, size_t, const Models::Joint&, const Models::Position& pos) {
            if (it.event() == flecs::OnSet) {

            }
            if (it.event() == flecs::OnAdd || it.event() == flecs::OnSet) {
                world.query<Models::Pin, Models::Position, Models::Rotation*>().each([pos](Models::Pin& pin, Models::Position& ePos, Models::Rotation* rot) {
                    if (pin.getAbs(ePos, rot) == pos) {

                    }
                });
            }
            // Check Network Component
    });
}

flecs::observer &pinDisconnector(const flecs::world &world) {

}