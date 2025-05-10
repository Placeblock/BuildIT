//
// Created by felix on 07.05.25.
//

#include "history/worldhistory.hpp"

#include <ranges>


flecs::entity_t EntityManager::createEntity(const GlobalEntityId globalEntityId) {
    const flecs::entity entity = this->world.entity();
    entity.set<GlobalEntityId>(globalEntityId);
    this->entities[globalEntityId] = entity;
    spdlog::debug("Created Entity with ID {}", entity.id());
    for (const auto lifecycleEventHandler : this->lifecycleEventHandlers) {
        lifecycleEventHandler->onEntityCreate(globalEntityId);
    }
    return entity;
}

void EntityManager::deleteEntity(const GlobalEntityId globalEntityId) {
    const flecs::entity entity = this->world.entity(this->entities.at(globalEntityId));
    if (!entity.is_alive()) {
        throw std::runtime_error("Tried to delete non-existing Entity!");
    }
    entity.destruct();
    this->entities.erase(globalEntityId);
    for (const auto lifecycleEventHandler : this->lifecycleEventHandlers) {
        lifecycleEventHandler->onEntityRemove(globalEntityId);
    }
    spdlog::debug("Deleted Entity with ID {}", entity.id());
}

flecs::entity_t EntityManager::getEntity(const GlobalEntityId globalEntityId) const {
    if (!this->entities.contains(globalEntityId)) {
        throw std::runtime_error("Tried to get non-existing Entity!");
    }
    return this->entities.at(globalEntityId);
}

bool EntityManager::hasEntity(const GlobalEntityId globalEntityId) const {
    return this->entities.contains(globalEntityId);
}

void EntityManager::addLifecycleEventHandler(EntityLifecycleEventHandler *handler) {
    this->lifecycleEventHandlers.insert(handler);
}

void EntityManager::removeLifecycleEventHandler(EntityLifecycleEventHandler *handler) {
    this->lifecycleEventHandlers.erase(handler);
}

flecs::entity_t EntityManager::getOrCreateEntity(const GlobalEntityId globalEntityId) {
    if (!this->entities.contains(globalEntityId)) {
        return this->createEntity(globalEntityId);
    }
    return this->entities.at(globalEntityId);
}

void EntityCreateChange::execute(EntityManager &entityManager, const flecs::world &world) const {
    entityManager.createEntity(this->entityId);
}

std::unique_ptr<Change> EntityCreateChange::inverse() const {
    std::unique_ptr<Change> inverse = std::make_unique<EntityRemoveChange>(this->entityId);
    inverse->oldEventId = this->newEventId;
    inverse->newEventId = this->oldEventId;
    return inverse;
}

void EntityRemoveChange::execute(EntityManager &entityManager, const flecs::world &world) const {
    entityManager.deleteEntity(this->entityId);
}

std::unique_ptr<Change> EntityRemoveChange::inverse() const {
    std::unique_ptr<Change> inverse = std::make_unique<EntityCreateChange>(this->entityId);
    inverse->oldEventId = this->newEventId;
    inverse->newEventId = this->oldEventId;
    return inverse;
}

bool WorldEvent::canExecute(const EntityStates &states) const {
    for (const auto& change : this->changes) {
        if (states.getEntityState(change->entityId) != change->oldEventId) {
            spdlog::debug("Cannot Execute WorldEvent because Entity {} is in State {} but should be in State {}.", change->entityId, states.getEntityState(change->entityId), change->oldEventId);
            return false;
        }
    }
    return true;
}

void WorldEvent::execute(EntityManager &entityManager, EntityStates &states, const flecs::world &world) const {
    if (!this->canExecute(states)) {
        throw std::runtime_error("Tried to execute WorldEvent that cannot be executed!");
    }
    for (const auto& change : this->changes) {
        change->execute(entityManager, world);
        states.pushEntityState(change->entityId, change->newEventId);
    }
}

std::unique_ptr<WorldEvent> WorldEvent::inverse() const {
    auto inverse = std::make_unique<WorldEvent>(this->type, this->player);
    for (const auto & change : std::ranges::reverse_view(this->changes)) {
        inverse->changes.push_back(change->inverse());
    }
    return inverse;
}

void PlayerWorldHistory::undo(EntityManager &entityManager, EntityStates &states, const flecs::world &world) {
    if (this->nextUndoIndex < 0) {
        throw std::runtime_error("Tried to undo while no undo is available!");
    }
    std::unique_ptr<WorldEvent> event = this->events[this->nextUndoIndex]->inverse();
    spdlog::debug("Starting Undo with {} Changes.", event->changes.size());
    event->execute(entityManager, states, world);
    this->events.push_back(std::move(event));
    this->nextUndoIndex--;
    spdlog::debug("Finished Undo.");
}

void PlayerWorldHistory::redo(EntityManager &entityManager, EntityStates &states, const flecs::world &world) {
    if (this->nextUndoIndex >= static_cast<int>(this->events.size())) {
        throw std::runtime_error("Tried to redo while no redo is available!");
    }
    const WorldEvent &event = *this->events[this->nextUndoIndex+1];
    spdlog::debug("Starting Redo with {} Changes.", event.changes.size());
    event.execute(entityManager, states, world);
    this->nextUndoIndex++;
    this->events.pop_back();
    spdlog::debug("Finished Redo.");
}

bool PlayerWorldHistory::canUndo() const {
    return this->nextUndoIndex >= 0;
}

bool PlayerWorldHistory::canRedo() const {
    return this->nextUndoIndex < this->events.size();
}

void EntityStates::pushEntityState(const GlobalEntityId entityId, const EventId eventId) {
    spdlog::debug("Updating Entity {}'s State to ID {}.", entityId, eventId);
    this->entityStates[entityId] = eventId;
}

EventId EntityStates::getEntityState(const GlobalEntityId entityId) const {
    if (!this->entityStates.contains(entityId)) {
        return 0;
    }
    return this->entityStates.at(entityId);
}

void WorldHistory::startEvent(std::unique_ptr<WorldEvent> event) {
    if (this->currentEvent != nullptr) {
        throw std::runtime_error("Tried to start new Event while another Event is still active!");
    }
    auto &[events, nextUndoIndex] = this->playerHistory[*event->player];
    event->id = ++this->nextEventId;
    spdlog::debug("Starting new History Event with ID {}.", event->id);
    this->currentEvent = event.get();
    events.emplace_back(std::move(event));
    nextUndoIndex = static_cast<int>(events.size()) - 1;
}

void WorldHistory::pushChange(std::unique_ptr<Change>& change) const {
    if (this->currentEvent == nullptr) {
        throw std::runtime_error("World Changed while no Event is active!");
    }
    change->oldEventId = this->entityStates.getEntityState(change->entityId);
    change->newEventId = this->currentEvent->id;
    this->currentEvent->changes.push_back(std::move(change));
}

void WorldHistory::endEvent() {
    if (this->currentEvent == nullptr) {
        throw std::runtime_error("Tried to end Event while no Event is active!");
    }
    for (const auto& change : this->currentEvent->changes) {
        this->entityStates.pushEntityState(change->entityId, this->currentEvent->id);
    }
    spdlog::debug("Finished History Event with {} Changes.", this->currentEvent->changes.size());
    this->currentEvent = nullptr;
}

PlayerWorldHistory * WorldHistory::getPlayerHistory(const Player player) {
    return &this->playerHistory[player];
}

void WorldHistory::receive(std::unique_ptr<WorldEvent> event, const std::function<void()> &runHandlers) {
    this->startEvent(std::move(event));
    runHandlers();
    this->endEvent();
}

WorldObserver::WorldObserver(const flecs::world &world, WorldHistory &worldHistory, EntityStates &entityStates, EntityManager &entityManager)
		: world(world), worldHistory(worldHistory), entityStates(entityStates), entityManager(entityManager) {
}

void WorldObserver::onEntityCreate(GlobalEntityId id) {
    std::unique_ptr<Change> change = std::make_unique<EntityCreateChange>(id);
    this->worldHistory.pushChange(change);
}

void WorldObserver::onEntityRemove(GlobalEntityId id) {
    std::unique_ptr<Change> change = std::make_unique<EntityRemoveChange>(id);
    this->worldHistory.pushChange(change);
}

void WorldObserver::runDisabled(const std::function<void()>& runnable) {
    for (const auto& observer : this->observers) {
        observer.disable();
    }
    this->entityManager.removeLifecycleEventHandler(this);
    runnable();
    this->entityManager.addLifecycleEventHandler(this);
    for (const auto& observer : this->observers) {
        observer.enable();
    }
}

WorldObserver::~WorldObserver() {
    for (auto observer : this->observers) {
        observer.destruct();
    }
}
