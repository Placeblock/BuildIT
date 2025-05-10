//
// Created by felix on 02.05.25.
//

#ifndef WORLDHISTORY_HPP
#define WORLDHISTORY_HPP
#include <string>

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

#include "event.hpp"
#include "history.hpp"

typedef uint64_t GlobalEntityId;

struct EntityLifecycleEventHandler {
	virtual ~EntityLifecycleEventHandler() = default;

	virtual void onEntityCreate(GlobalEntityId id) = 0;
	virtual void onEntityRemove(GlobalEntityId id) = 0;
};
class EntityManager {
	flecs::world &world;
	std::unordered_map<GlobalEntityId, flecs::entity_t> entities;
	std::unordered_set<EntityLifecycleEventHandler*> lifecycleEventHandlers;

public:
	explicit EntityManager(flecs::world &world) : world(world) {}

	flecs::entity_t getOrCreateEntity(GlobalEntityId globalEntityId);
	flecs::entity_t createEntity(GlobalEntityId globalEntityId);
	void deleteEntity(GlobalEntityId globalEntityId);
	[[nodiscard]] flecs::entity_t getEntity(GlobalEntityId globalEntityId) const;
	[[nodiscard]] bool hasEntity(GlobalEntityId globalEntityId) const;

	void addLifecycleEventHandler(EntityLifecycleEventHandler* handler);
	void removeLifecycleEventHandler(EntityLifecycleEventHandler* handler);
};

typedef uint32_t EventId;

struct EntityStates {
	std::unordered_map<GlobalEntityId, EventId> entityStates;

	void pushEntityState(GlobalEntityId entityId, EventId eventId);
	[[nodiscard]] EventId getEntityState(GlobalEntityId entityId) const;
};

struct Change {
	const GlobalEntityId entityId;
	uint32_t oldEventId = 0;
	uint32_t newEventId = 0;

	explicit Change(const GlobalEntityId entityId) : entityId(entityId) {}
	virtual ~Change() = default;

	virtual void execute(EntityManager& entityManager, const flecs::world &world) const = 0;
	[[nodiscard]] virtual std::unique_ptr<Change> inverse() const = 0;
};

class EntityCreateChange final : public Change {
public:
	explicit EntityCreateChange(const GlobalEntityId entityId) : Change(entityId) {}

	void execute(EntityManager& entityManager, const flecs::world &world) const override;
	[[nodiscard]] std::unique_ptr<Change> inverse() const override;
};

class EntityRemoveChange final : public Change {
public:
	explicit EntityRemoveChange(const GlobalEntityId entityId) : Change(entityId) {}

	void execute(EntityManager& entityManager, const flecs::world &world) const override;
	[[nodiscard]] std::unique_ptr<Change> inverse() const override;
};

template<std::equality_comparable T>
class ComponentChange final : public Change {
	std::unique_ptr<T> oldValue;
	std::unique_ptr<T> newValue;

public:
	ComponentChange() = delete;
	ComponentChange(const GlobalEntityId entityId, std::unique_ptr<T>& oldValue, std::unique_ptr<T>& newValue)
		: Change(entityId), oldValue(std::move(oldValue)), newValue(std::move(newValue)) {}

	void execute(EntityManager& entityManager, const flecs::world &world) const override {
		const flecs::entity_t entityId = entityManager.getEntity(this->entityId);
		const flecs::entity entity = world.entity(entityId);
		if (this->newValue == nullptr) {
			spdlog::debug("Removing Component {}", typeid(T).name());
			entity.remove<T>();
		} else {
			spdlog::debug("Updating Component {}", typeid(T).name());
			entity.set(*this->newValue);
		}
	}

	[[nodiscard]] std::unique_ptr<Change> inverse() const override {
		std::unique_ptr<T> oldValue = nullptr;
		if (this->newValue != nullptr) {
			oldValue = std::make_unique<T>(*this->newValue);
		}
		std::unique_ptr<T> newValue = nullptr;
		if (this->oldValue != nullptr) {
			newValue = std::make_unique<T>(*this->oldValue);
		}
		std::unique_ptr<Change> change = std::make_unique<ComponentChange>(this->entityId, oldValue, newValue);
		change->oldEventId = this->newEventId;
		change->newEventId = this->oldEventId;
		return change;
	}
};

struct WorldEvent : Event {
	std::vector<std::unique_ptr<Change>> changes;
	EventId id = 0;

	explicit WorldEvent(const EventType& type, const Player *player) : Event(type, player) {}

	[[nodiscard]] bool canExecute(const EntityStates &states) const;
	void execute(EntityManager& entityManager, EntityStates &states, const flecs::world &world) const;
	[[nodiscard]] std::unique_ptr<WorldEvent> inverse() const;
};

struct PlayerWorldHistory {
	std::vector<std::unique_ptr<WorldEvent>> events;
	int nextUndoIndex = 0;

	void undo(EntityManager &entityManager, EntityStates &states, const flecs::world &world);
	void redo(EntityManager &entityManager, EntityStates &states, const flecs::world &world);
	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;
};

class WorldObserver;

class WorldHistory final : public History<WorldEvent> {
	friend WorldObserver;

	EntityStates &entityStates;
	std::unordered_map<Player, PlayerWorldHistory> playerHistory;
	WorldEvent *currentEvent = nullptr;
	EventId nextEventId = 0;

	void startEvent(std::unique_ptr<WorldEvent> event);
	void pushChange(std::unique_ptr<Change>& change) const;
	void endEvent();
public:
	explicit WorldHistory(EntityStates &entity_states)
		: entityStates(entity_states) {}

	PlayerWorldHistory* getPlayerHistory(Player player);

	void receive(std::unique_ptr<WorldEvent> event, const std::function<void()>& runHandlers) override;
};

template<typename T>
struct OldValue {
	T value;
};

class WorldObserver final : public EntityLifecycleEventHandler {
	const flecs::world &world;
	WorldHistory &worldHistory;
	EntityStates &entityStates;
	EntityManager& entityManager;
	std::vector<flecs::observer> observers;
public:
	WorldObserver(const flecs::world &world, WorldHistory &worldHistory, EntityStates &entityStates, EntityManager& entityManager);

	void onEntityCreate(GlobalEntityId id) override;
	void onEntityRemove(GlobalEntityId id) override;

	template<typename T>
	void observe() {
		this->observers.push_back(this->world.observer<T, GlobalEntityId>()
			.event(flecs::OnSet)
			.event(flecs::OnRemove)
			.each([this](const flecs::iter& it, const size_t i, T& data, GlobalEntityId& id) {
				spdlog::debug("Updating Component {}", typeid(T).name());
				auto entity = it.entity(i);
				auto oldValuePtr = entity.get<OldValue<T>>();
				std::unique_ptr<T> oldValue = nullptr;
				if (oldValuePtr != nullptr) {
					oldValue = std::make_unique<T>(oldValuePtr->value);
				}
				std::unique_ptr<T> newValue = nullptr;
				if (it.event() == flecs::OnSet) {
					newValue = std::make_unique<T>(data);
				}
				std::unique_ptr<Change> change = std::make_unique<ComponentChange<T>>(id, oldValue, newValue);
				this->worldHistory.pushChange(change);
				entity.set<OldValue<T>>(OldValue<T>{data});
			})
		);
	}

	void runDisabled(const std::function<void()>& runnable);

	~WorldObserver() override;
};

#endif //WORLDHISTORY_HPP
