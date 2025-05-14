//
// Created by felix on 02.05.25.
//

#ifndef WORLDHISTORY_HPP
#define WORLDHISTORY_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

#include "event.hpp"
#include "history.hpp"
#include "model/components.hpp"

typedef uint64_t GlobalEntityId;

struct EntityLifecycleEventHandler {
	virtual ~EntityLifecycleEventHandler() = default;

	virtual void onEntityCreate(GlobalEntityId id) = 0;
	virtual void onEntityRemove(GlobalEntityId id) = 0;
};
class EntityManager {
    BuildIT::Registry& registry;
    std::unordered_map<GlobalEntityId, BuildIT::Entity> entities;
	std::unordered_set<EntityLifecycleEventHandler*> lifecycleEventHandlers;

public:
    explicit EntityManager(BuildIT::Registry &registry) : registry(registry) {}

    BuildIT::Entity getOrCreateEntity(GlobalEntityId globalEntityId);
    BuildIT::Entity createEntity(GlobalEntityId globalEntityId);
	void deleteEntity(GlobalEntityId globalEntityId);
    [[nodiscard]] BuildIT::Entity getEntity(GlobalEntityId globalEntityId) const;
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

    virtual void execute(EntityManager& entityManager, const BuildIT::Registry &registry) const = 0;
	[[nodiscard]] virtual std::unique_ptr<Change> inverse() const = 0;
};

class EntityCreateChange final : public Change {
public:
	explicit EntityCreateChange(const GlobalEntityId entityId) : Change(entityId) {}

    void execute(EntityManager& entityManager, const BuildIT::Registry &registry) const override;
	[[nodiscard]] std::unique_ptr<Change> inverse() const override;
};

class EntityRemoveChange final : public Change {
public:
	explicit EntityRemoveChange(const GlobalEntityId entityId) : Change(entityId) {}

    void execute(EntityManager& entityManager, const BuildIT::Registry &registry) const override;
	[[nodiscard]] std::unique_ptr<Change> inverse() const override;
};

template<std::equality_comparable T>
class ComponentReplaceChange final : public Change {
    T oldValue;
    T newValue;

public:
    ComponentReplaceChange() = delete;
    ComponentReplaceChange(const GlobalEntityId entityId, T oldValue, T newValue)
        : Change(entityId), oldValue(oldValue), newValue(newValue) {}

    void execute(EntityManager& entityManager, const BuildIT::Registry &registry) const override {
        const BuildIT::Entity entity = entityManager.getEntity(this->entityId);
        spdlog::debug("Replacing Component {} in Entity {}", typeid(T).name(), this->entityId);
        registry.replace<T>(entity, *this->newValue);
    }

    [[nodiscard]] std::unique_ptr<Change> inverse() const override {
        return ComponentReplaceChange{this->entityId, this->newValue, this->oldValue};
    }
};

template<std::equality_comparable T>
class ComponentEraseChange;

template<std::equality_comparable T>
class ComponentEmplaceChange final : public Change {
    T newValue;

public:
    ComponentEmplaceChange() = delete;
    ComponentEmplaceChange(const GlobalEntityId entityId, T newValue)
        : Change(entityId), newValue(newValue) {}

    void execute(EntityManager &entityManager, const BuildIT::Registry &registry) const override {
        const BuildIT::Entity entity = entityManager.getEntity(this->entityId);
        spdlog::debug("Emplacing Component {} in Entity {}", typeid(T).name(), this->entityId);
        registry.emplace<T>(entity, *this->newValue);
    }

    [[nodiscard]] std::unique_ptr<Change> inverse() const override {
        return ComponentEraseChange{this->entityId, this->newValue};
    }
};

template<std::equality_comparable T>
class ComponentEraseChange final : public Change {
    T oldValue;

public:
    ComponentEraseChange() = delete;
    ComponentEraseChange(const GlobalEntityId entityId, T oldValue)
        : Change(entityId), oldValue(oldValue) {}

    void execute(EntityManager &entityManager, const BuildIT::Registry &registry) const override {
        const BuildIT::Entity entity = entityManager.getEntity(this->entityId);
        spdlog::debug("Emplacing Component {} in Entity {}", typeid(T).name(), this->entityId);
        registry.erase<T>(entity);
    }

    [[nodiscard]] std::unique_ptr<Change> inverse() const override {
        return ComponentEmplaceChange{this->entityId, this->oldValue};
    }
};

struct WorldEvent : Event::Event {
	std::vector<std::unique_ptr<Change>> changes;
	EventId id = 0;

    explicit WorldEvent(const ::Event::EventType& type, const BuildIT::Player *player) : Event(type, player) {}

	[[nodiscard]] bool canExecute(const EntityStates &states) const;
    void execute(EntityManager& entityManager, EntityStates &states, const BuildIT::Registry &registry) const;
	[[nodiscard]] std::unique_ptr<WorldEvent> inverse() const;
};

struct PlayerWorldHistory {
	std::vector<std::unique_ptr<WorldEvent>> events;
	int nextUndoIndex = 0;

    void undo(EntityManager &entityManager, EntityStates &states, const BuildIT::Registry &registry);
    void redo(EntityManager &entityManager, EntityStates &states, const BuildIT::Registry &registry);
	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;
};

class WorldObserver;

class WorldHistory final : public History::History<WorldEvent> {
	friend WorldObserver;

	EntityStates &entityStates;
    std::unordered_map<BuildIT::Player, PlayerWorldHistory> playerHistory;
	WorldEvent *currentEvent = nullptr;
	EventId nextEventId = 0;

	void startEvent(std::unique_ptr<WorldEvent> event);
	void pushChange(std::unique_ptr<Change>& change) const;
	void endEvent();
public:
	explicit WorldHistory(EntityStates &entity_states)
		: entityStates(entity_states) {}

    PlayerWorldHistory* getPlayerHistory(BuildIT::Player player);

	void receive(std::unique_ptr<WorldEvent> event, const std::function<void()>& runHandlers) override;
};

template<typename T>
struct OldValue {
    T value;
};

struct ComponentObserver {
    virtual void onConstruct(BuildIT::Registry &registry, BuildIT::Entity entity) const = 0;
    virtual void onUpdate(BuildIT::Registry &registry, BuildIT::Entity entity) const = 0;
    virtual void onDestroy(BuildIT::Registry &registry, BuildIT::Entity entity) const = 0;
    virtual void connect(const BuildIT::Registry &registry) const = 0;
    virtual void disconnect(const BuildIT::Registry &registry) const = 0;
};
template<typename T>
struct BasicComponentObserver final : public ComponentObserver
{
    BasicComponentObserver(std::function<void(std::unique_ptr<ComponentChange<T>> values)> callback)
        : callback(callback) {};
    void onConstruct(BuildIT::Registry &registry, BuildIT::Entity entity) const override
    {
        spdlog::debug("Observing Component Creation {} for Entity {}", typeid(T).name(), entity);
        T &data = registry.get<T>(entity);
        OldValue<T> &oldValueRef = registry.get<OldValue<T>>();
        std::unique_ptr<T> newValue = std::make_unique<T>(data);
        std::unique_ptr<ComponentChange<T>> values = std::make_unique(std::make_unique<T>(),
                                                                      newValue);
        registry.emplace<OldValue<T>>(entity, OldValue<T>{data});
        this->callback(values);
    }
    void onUpdate(BuildIT::Registry &registry, BuildIT::Entity entity) const override
    {
        spdlog::debug("Observing Component Update {} for Entity {}", typeid(T).name(), entity);
        T &data = registry.get<T>(entity);
        OldValue<T> &oldValueRef = registry.get<OldValue<T>>();
        std::unique_ptr<T> oldValue = std::make_unique<T>(oldValueRef.value);
        std::unique_ptr<T> newValue = std::make_unique<T>(data);
        std::unique_ptr<ComponentChange<T>> values = std::make_unique(oldValue, newValue);
        registry.replace<OldValue<T>>(entity, OldValue<T>{data});
        this->callback(values);
    }
    void onDestroy(BuildIT::Registry &registry, BuildIT::Entity entity) const override
    {
        spdlog::debug("Observing Component Deletion {} for Entity {}", typeid(T).name(), entity);
        T &data = registry.get<T>(entity);
        OldValue<T> &oldValueRef = registry.get<OldValue<T>>();
        std::unique_ptr<T> oldValue = std::make_unique<T>(oldValueRef.value);
        std::unique_ptr<ComponentChange<T>> values = std::make_unique(oldValue,
                                                                      std::make_unique<T>());
        registry.destroy<OldValue<T>>(entity);
        this->callback(values);
    }
    void connect(const BuildIT::Registry &registry) const override
    {
        registry.on_construct<T>().connect<&BasicComponentObserver<T>::onConstruct>(*this);
        registry.on_update<T>().connect<&BasicComponentObserver<T>::onUpdate>(*this);
        registry.on_destroy<T>().connect<&BasicComponentObserver<T>::onDestroy>(*this);
    }
    void disconnect(const BuildIT::Registry &registry) const override
    {
        registry.on_construct<T>().disconnect<&BasicComponentObserver<T>::onConstruct>(*this);
        registry.on_update<T>().disconnect<&BasicComponentObserver<T>::onUpdate>(*this);
        registry.on_destroy<T>().disconnect<&BasicComponentObserver<T>::onDestroy>(*this);
    }

private:
    std::function<void(std::unique_ptr<ComponentChange<T>> change)> callback;
};

class WorldObserver final
{
public:
    WorldObserver(const BuildIT::Registry &registry,
                  WorldHistory &worldHistory,
                  EntityStates &entityStates,
                  EntityManager &entityManager);

    template<typename T>
    void observe()
    {
        auto observer = std::make_unique<BasicComponentObserver<T>>(
            [this](std::unique_ptr<ComponentChange<T>> change) {
                this->worldHistory.pushChange(change);
            });
        observer->connect(this->registry);
    }

    void runDisabled(const std::function<void()> &runnable);

    ~WorldObserver();

private:
    const BuildIT::Registry &registry;
    WorldHistory &worldHistory;
    EntityStates &entityStates;
    EntityManager &entityManager;
    std::vector<std::unique_ptr<ComponentObserver>> observers;
};

#endif //WORLDHISTORY_HPP
