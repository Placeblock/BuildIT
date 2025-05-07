//
// Created by felix on 02.05.25.
//

#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <string>

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include <vector>

typedef uint64_t GlobalEntityId;

class EntityManager {
	std::unordered_map<GlobalEntityId, flecs::entity_t> entities;

public:
	flecs::entity_t createEntity(GlobalEntityId globalEntityId);
	void deleteEntity(GlobalEntityId globalEntityId);
	flecs::entity_t getEntity(GlobalEntityId globalEntityId);
};

class BaseComponentChange {
	virtual ~BaseComponentChange() = default;

protected:
	const GlobalEntityId entityId;
	const flecs::entity_t componentId;

public:
	BaseComponentChange(const GlobalEntityId entityId, const flecs::entity_t componentId)
		: entityId(entityId), componentId(componentId) {}

	virtual void undo(const EntityManager& entityManager, const flecs::world &world) const = 0;
	virtual void redo(const EntityManager& entityManager, const flecs::world &world) const = 0;
};

template<typename T>
class ComponentChange<T> final : BaseComponentChange {
	std::unique_ptr<T> oldValue;
	std::unique_ptr<T> newValue;

public:
	ComponentChange(const GlobalEntityId entityId, const flecs::entity_t componentId, std::unique_ptr<T> oldValue, std::unique_ptr<T> newValue)
	: BaseComponentChange(entityId, componentId), oldValue(std::move(oldValue)), newValue(std::move(newValue)) {}
	void undo(const EntityManager& entityManager, const flecs::world &world) const override;
	void redo(const EntityManager& entityManager, const flecs::world &world) const override;
};

class Player {};

struct EventMetadata {
	std::string type;
	std::string name;
	std::string description;
	Player *player;
};

struct Event {
	EventMetadata metadata;
	std::vector<std::unique_ptr<BaseComponentChange>> changes;
};

class History {
	std::vector<Event> events;
	Event *currentEvent = nullptr;

	void startEvent(const EventMetadata& metadata);
	void pushChange(std::unique_ptr<BaseComponentChange> change) const;
	void endEvent();
};

class WorldObserver {
	const flecs::world &world;
	std::vector<flecs::observer> observers;
public:
	WorldObserver(const flecs::world &world)
		: world(world) {}

	template<typename T>
	void observe();
};

#endif //HISTORY_HPP
