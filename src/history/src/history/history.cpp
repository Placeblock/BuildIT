//
// Created by felix on 07.05.25.
//

#include "history/history.hpp"


flecs::entity_t EntityManager::createEntity(const GlobalEntityId globalEntityId) {
    const flecs::entity entity = this->world.entity();
    entity.set<GlobalEntityId>(globalEntityId);
    this->entities[globalEntityId] = entity;
    spdlog::debug("Created Entity with ID {}", entity.id());
    return entity;
}

void EntityManager::deleteEntity(const GlobalEntityId globalEntityId) {
    const flecs::entity entity = this->world.entity(this->entities.at(globalEntityId));
    if (!entity.is_alive()) {
        throw std::runtime_error("Tried to delete non-existing Entity!");
    }
    entity.destruct();
    this->entities.erase(globalEntityId);
    spdlog::debug("Deleted Entity with ID {}", entity.id());
}

flecs::entity_t EntityManager::getEntity(const GlobalEntityId globalEntityId) const {
    return this->entities.at(globalEntityId);
}
flecs::entity_t EntityManager::getOrCreateEntity(const GlobalEntityId globalEntityId) {
    if (this->entities.find(globalEntityId) == this->entities.end()) {
        return this->createEntity(globalEntityId);
    }
    return this->entities.at(globalEntityId);
}

void WorldEvent::execute(EntityManager &entityManager, const flecs::world &world) const {
    for (const auto& change : this->changes) {
        change->execute(entityManager, world);
    }
}

std::unique_ptr<WorldEvent> WorldEvent::inverse() const {
    auto inverse = std::make_unique<WorldEvent>(this->metadata);
    for (auto it = this->changes.rbegin(); it != this->changes.rend(); ++it) {
        inverse->changes.push_back((*it)->inverse());
    }
    return inverse;
}

void PlayerWorldHistory::undo(EntityManager &entityManager, const flecs::world &world) {
    if (this->nextUndoIndex < 0) {
        throw std::runtime_error("Tried to undo while no undo is available!");
    }
    spdlog::debug("Undo Event from Player in World");
    std::unique_ptr<WorldEvent> event = this->events[this->nextUndoIndex]->inverse();;
    event->execute(entityManager, world);
    this->events.push_back(std::move(event));
    this->nextUndoIndex--;
}

void PlayerWorldHistory::redo(EntityManager &entityManager, const flecs::world &world) {
    if (this->nextUndoIndex >= this->events.size()) {
        throw std::runtime_error("Tried to redo while no redo is available!");
    }
    spdlog::debug("Redo Event from Player in World");
    const WorldEvent &event = *this->events[this->nextUndoIndex+1];
    event.execute(entityManager, world);
    this->nextUndoIndex++;
    this->events.pop_back();
}

bool PlayerWorldHistory::canUndo() const {
    return this->nextUndoIndex >= 0;
}

bool PlayerWorldHistory::canRedo() const {
    return this->nextUndoIndex < this->events.size();
}

void WorldHistory::startEvent(const WorldEventMetadata& metadata) {
    spdlog::debug("Starting new History Event.");
    if (this->currentEvent != nullptr) {
        throw std::runtime_error("Tried to start new Event while another Event is still active!");
    }
    this->playerHistory[*metadata.player].events.emplace_back(std::make_unique<WorldEvent>(metadata));
    this->playerHistory[*metadata.player].nextUndoIndex = this->playerHistory[*metadata.player].events.size() - 1;
    this->currentEvent = this->playerHistory[*metadata.player].events.back().get();
}

void WorldHistory::pushChange(std::unique_ptr<BaseComponentChange>& change) const {
    this->currentEvent->changes.push_back(std::move(change));
}

void WorldHistory::endEvent() {
    spdlog::debug("Ending new History Event.");
    if (this->currentEvent == nullptr) {
        throw std::runtime_error("Tried to end Event while no Event is active!");
    }
    this->currentEvent = nullptr;
}

WorldObserver::~WorldObserver() {
    for (auto observer : this->observers) {
        observer.destruct();
    }
}
