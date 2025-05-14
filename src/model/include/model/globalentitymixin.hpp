#ifndef GLOBALENTITYREGISTRY_HPP
#define GLOBALENTITYREGISTRY_HPP

#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace buildit {
namespace ecs {

/**
 * @brief Adds global-entity-id support to a registry.
 */
template<typename Type, typename GlobalEntity>
class global_entity_mixin : public Type {
    using global_entity_type = GlobalEntity;
    using underlying_type = Type;
    using entity_type = underlying_type::entity_type;
    using version_type = underlying_type::version_type;

    entity_type create(const global_entity_type& entity_id) {
        if (this->entities.contains(entity_id)) {
            spdlog::error("Tried to create already existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-creation failed"};
        }
        const entity_type& new_entity = this->create();
        this->entities[entity_id] = new_entity;
        return new_entity;
    }

    version_type destroy(const global_entity_type& entity_id) {
        if (!this->entities.contains(entity_id)) {
            spdlog::error("Tried to remove non-existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-destruction failed"};
        }
        const entity_type old_entity = this->entities[entity_id];
        this->entities.erase(entity_id);
        return this->destroy(old_entity);
    }

    entity_type get(const global_entity_type& entity_id) {
        if (!this->entities.contains(entity_id)) {
            spdlog::error("Tried to access non-existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-access failed"};
        }
        return this->entities[entity_id];
    }

    void swap(global_entity_mixin<underlying_type, global_entity_type>& other) noexcept {
        using std::swap;
        swap(entities, other.entities);
        underlying_type::swap(other);
    }

private:
    std::unordered_map<global_entity_type, entity_type> entities;
};
} // namespace buildit

#endif // GLOBALENTITYREGISTRY_HPP
