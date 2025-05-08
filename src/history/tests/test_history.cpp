//
// Created by felix on 08.05.25.
//

#include <cstdio>

#include <flecs.h>

#include "history/history.hpp"

struct TestComponent {
    int32_t value;
};

int main() {
    flecs::world world;

    EntityManager manager{world};
    const flecs::entity_t entityId = manager.createEntity(0);
    const flecs::entity entity = world.entity(entityId);

    Player player = 1;

    WorldHistory history;
    WorldObserver observer{world, history};
    observer.observe<TestComponent>(); // Track Component in History

    const WorldEventMetadata metadata{"de.codelix.buildit.testevent",
        "Test Event", "Thats a neat little Test Event", &player};
    history.startEvent(metadata);
    entity.set<TestComponent>({1});
    history.endEvent();


}
