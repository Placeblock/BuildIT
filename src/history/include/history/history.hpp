//
// Created by felix on 02.05.25.
//

#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <string>

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <stdexcept>
#include <spdlog/spdlog.h>

typedef uint64_t GlobalEntityId;

class EntityManager {
	flecs::world &world;
	std::unordered_map<GlobalEntityId, flecs::entity_t> entities;

public:
	explicit EntityManager(flecs::world &world) : world(world) {}

	flecs::entity_t getOrCreateEntity(GlobalEntityId globalEntityId);
	flecs::entity_t createEntity(GlobalEntityId globalEntityId);
	void deleteEntity(GlobalEntityId globalEntityId);
	[[nodiscard]] flecs::entity_t getEntity(GlobalEntityId globalEntityId) const;
};

class BaseComponentChange {

protected:
	const GlobalEntityId entityId;

public:
	explicit BaseComponentChange(const GlobalEntityId entityId) : entityId(entityId)  {}
	virtual ~BaseComponentChange() = default;

	virtual void execute(EntityManager& entityManager, const flecs::world &world) const = 0;
	[[nodiscard]] virtual std::unique_ptr<BaseComponentChange> inverse() const = 0;
};

template<typename T>
class ComponentChange final : public BaseComponentChange {
	std::unique_ptr<T> oldValue;
	std::unique_ptr<T> newValue;

public:
	ComponentChange() = delete;
	ComponentChange(const GlobalEntityId entityId, std::unique_ptr<T>& oldValue, std::unique_ptr<T>& newValue)
		: BaseComponentChange(entityId), oldValue(std::move(oldValue)), newValue(std::move(newValue)) {}
	void execute(EntityManager& entityManager, const flecs::world &world) const override {
		const flecs::entity_t entityId = entityManager.getOrCreateEntity(this->entityId);
		const flecs::entity entity = world.entity(entityId);
		if (this->newValue == nullptr) {
			entity.remove<T>();
		} else {
			entity.set<T>(*this->newValue);
		}
		// Delete Entity if it has no more Components attached to it.
		if (entity.type().count() == 0) {
			spdlog::debug("Deleting Entity {} because it has no more Components attached to it.", this->entityId);
			entityManager.deleteEntity(this->entityId);
		}
	}

	[[nodiscard]] std::unique_ptr<BaseComponentChange> inverse() const override {
		std::unique_ptr<T> oldValue = std::make_unique<T>(*this->newValue);
		std::unique_ptr<T> newValue = std::make_unique<T>(*this->oldValue);
		return std::make_unique<ComponentChange>(this->entityId, oldValue, newValue);
	}
};

typedef uint64_t Player;

struct WorldEventMetadata {
	std::string type;
	std::string name;
	std::string description;
	Player *player;

	WorldEventMetadata() = delete;
};

struct WorldEvent {
	WorldEventMetadata metadata;
	std::vector<std::unique_ptr<BaseComponentChange>> changes;

	explicit WorldEvent(WorldEventMetadata metadata)
		: metadata(std::move(metadata)) {}

	void execute(EntityManager& entityManager, const flecs::world &world) const;
	[[nodiscard]] std::unique_ptr<WorldEvent> inverse() const;
};

struct PlayerWorldHistory {
	std::vector<std::unique_ptr<WorldEvent>> events;
	ssize_t nextUndoIndex = 0;

	void undo(EntityManager &entityManager, const flecs::world &world);
	void redo(EntityManager &entityManager, const flecs::world &world);
	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;
};

class WorldObserver;

class WorldHistory {
	friend WorldObserver;
	std::unordered_map<Player, PlayerWorldHistory> playerHistory;
	WorldEvent *currentEvent = nullptr;
	void pushChange(std::unique_ptr<BaseComponentChange>& change) const;

public:
	void startEvent(const WorldEventMetadata& metadata);
	void endEvent();
};

template<typename T>
struct OldValue {
	T value;
};

class WorldObserver {
	const flecs::world &world;
	const WorldHistory &worldHistory;
	std::vector<flecs::observer> observers;
public:
	WorldObserver(const flecs::world &world, const WorldHistory &worldHistory)
		: world(world), worldHistory(worldHistory) {}

	template<typename T>
	void observe() {
		this->observers.push_back(this->world.observer<T>()
			.event(flecs::OnSet)
			.event(flecs::OnRemove)
			.each([this](const flecs::iter& it, const size_t i, T& data) {
				auto entity = it.entity(i);
				auto globalEntityId = entity.get<GlobalEntityId>();
				if (globalEntityId == nullptr) {
					throw std::runtime_error("Tried to observe a Component without GlobalEntityId!");
				}
				auto oldValuePtr = entity.get<OldValue<T>>();
				std::unique_ptr<T> oldValue;
				if (oldValuePtr != nullptr) {
					oldValue = std::make_unique<T>(oldValuePtr->value);
				}
				std::unique_ptr<T> newValue;
				if (it.event() == flecs::OnSet) {
					newValue = std::make_unique<T>(data);
				}
				std::unique_ptr<BaseComponentChange> change = std::make_unique<ComponentChange<T>>(*globalEntityId, oldValue, newValue);
				this->worldHistory.pushChange(change);
				entity.set<OldValue<T>>(OldValue<T>{data});
			})
		);
	}

	~WorldObserver();
};

#endif //HISTORY_HPP
