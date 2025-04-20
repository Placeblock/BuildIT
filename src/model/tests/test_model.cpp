//
// Created by felix on 20.04.25.
//

#include <cstdio>

#include <flecs.h>

#include "../include/model/network.hpp"

int main() {
    const flecs::world world;

    const flecs::entity parent = world.entity("Parent");
    const flecs::entity child = world.entity("Child");
    parent.set<Models::Connection>(child, {true, 0});
    child.set<Models::Connection>(parent, {false, 1});

    /*const auto q = world.query_builder<>()
        .with(world.id<Models::Connection>(), flecs::Wildcard)
        .build();
    q.each([&](auto entity) {
        printf("entity: %s\n", entity.name());
    });*/
    int32_t index = 0;
    flecs::entity found;
    while ((found = parent.target(world.id<Models::Connection>(), index++))) {
        printf("Name: %s\n", found.name().c_str());
    }

}
