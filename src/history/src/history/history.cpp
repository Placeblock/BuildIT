//
// Created by felix on 07.05.25.
//

#include "spdlog/spdlog.h"
#include "history/history.hpp"

#include <stdexcept>

template<typename T>
void ComponentChange<T>::undo(const EntityManager& entityManager, const flecs::world &world) const {
    flecs::entity entity = entityManager.getEntity(this->entityId);
    if (!world.is_alive(entity)) {
        entity = world.make_alive(entity);
    }
    entity.set<T>(this->oldValue);
}

template<typename T>
void ComponentChange<T>::redo(const EntityManager& entityManager, const flecs::world &world) const {
    flecs::entity entity = entityManager.getEntity(this->entityId);
    if (!world.is_alive(entity)) {
        throw std::runtime_error("Tried to redo Component Change on invalid Entity!");
    }
    entity.set<T>(this->newValue);
    // Delete Entity if it has no more Components attached to it.
    if (entity.type().count() == 0) {
        spdlog::debug("Deleting Entity {} because it has no more Components attached to it.", this->entityId);
        entity.destruct();
    }
}

void History::startEvent(const EventMetadata& metadata) {
    spdlog::debug("Starting new History Event.");
    if (this->currentEvent != nullptr) {
        throw std::runtime_error("Tried to start new Event while another Event is still active!");
    }
    this->events.push_back({metadata});
    this->currentEvent = &this->events.back();
}

void History::pushChange(std::unique_ptr<BaseComponentChange> change) const {
    this->currentEvent->changes.push_back(std::move(change));
}

void History::endEvent() {
    spdlog::debug("Ending new History Event.");
    if (this->currentEvent == nullptr) {
        throw std::runtime_error("Tried to end Event while no Event is active!");
    }
    this->currentEvent = nullptr;
}

template<typename T>
void WorldObserver::observe() {
    this->world.observer()
}
