//
// Created by felix on 08.05.25.
//

#include <flecs.h>

#include "history/history.hpp"

struct TestComponent {
    int32_t value;
};

int main() {
    spdlog::set_level(spdlog::level::trace);
    flecs::world world;

    EntityManager manager{world};

    Player player = 1;

    WorldHistory history;
    WorldObserver observer{world, history, manager};
    manager.addLifecycleEventHandler(&observer);
    observer.observe<TestComponent>(); // Track Component in History

    const WorldEventMetadata metadata2{"de.codelix.buildit.testevent",
        "Test Event", "Thats a neat little Test Event", &player};
    history.captureChanges(metadata2, [&manager, &world] mutable {
        const flecs::entity_t entityId = manager.createEntity(0);
        const flecs::entity entity = world.entity(entityId);
        entity.set<TestComponent>({1});
    });

    observer.runDisabled([&history, &player, &manager, &world]() mutable {
        history.getPlayerHistory(player)->undo(manager, world);
        history.getPlayerHistory(player)->redo(manager, world);
        history.getPlayerHistory(player)->undo(manager, world);
    });

    try {
        manager.getEntity(0);
    } catch (std::runtime_error& e) {
        return 1;
    }
    throw std::runtime_error("Entity still exists");
}
