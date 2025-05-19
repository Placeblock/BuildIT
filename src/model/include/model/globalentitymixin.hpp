#ifndef GLOBALENTITYREGISTRY_HPP
#define GLOBALENTITYREGISTRY_HPP

#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace buildit {
namespace ecs {

/**
 * @brief Adds global-entity-id support to a registry.
 * 
 * Provides methods to create get and delete entities using the global entity id.
 */
template<typename Type, typename GlobalEntity>
class global_entity_mixin : public Type {
    using global_entity_type = GlobalEntity;
    using underlying_type = Type;
    using entity_type = underlying_type::entity_type;
    using version_type = underlying_type::version_type;
    using allocator_type = underlying_type::allocator_type;
    using size_type = underlying_type::size_type;

    using sigh_type = entt::sigh<void(underlying_type &, GlobalEntity &),
                                 typename underlying_type::allocator_type>;

public:
    /*! @brief Default constructor. */
    global_entity_mixin() : global_entity_mixin{allocator_type{}} {}

    /**
     * @brief Constructs an empty registry with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit global_entity_mixin(const allocator_type &allocator)
        : global_entity_mixin{0u, allocator} {}

    /**
     * @brief Allocates enough memory upon construction to store `count` pools.
     * @param count The number of pools to allocate memory for.
     * @param allocator The allocator to use.
     */
    global_entity_mixin(const size_type count, const allocator_type &allocator = allocator_type{})
        : underlying_type(count, allocator), create_sigh{allocator}, destroy_sigh{allocator} {}

    /*! @brief Default copy constructor, deleted on purpose. */
    global_entity_mixin(const global_entity_mixin &) = delete;

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    global_entity_mixin(global_entity_mixin &&other) noexcept
        : underlying_type{std::move(other)}
        , create_sigh{std::move(other.create_sigh)}
        , destroy_sigh{std::move(other.destroy_sigh)} {}

    /*! @brief Default destructor. */
    ~global_entity_mixin() = default;

    /**
     * @brief Default copy assignment operator, deleted on purpose.
     * @return This mixin.
     */
    global_entity_mixin &operator=(const global_entity_mixin &) = delete;

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This registry.
     */
    global_entity_mixin &operator=(global_entity_mixin &&other) noexcept {
        swap(other);
        return *this;
    }

    entity_type create(const global_entity_type &entity_id) {
        if (this->global_entities.contains(entity_id)) {
            spdlog::error("Tried to create already existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-creation failed"};
        }
        const entity_type& new_entity = this->create();
        this->global_entities[entity_id] = new_entity;
        this->create_sigh.publish(*this, entity_id);
        return new_entity;
    }

    version_type destroy(const global_entity_type& entity_id) {
        if (!this->global_entities.contains(entity_id)) {
            spdlog::error("Tried to remove non-existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-destruction failed"};
        }
        const entity_type old_entity = this->global_entities[entity_id];
        this->global_entities.erase(entity_id);
        const auto version = this->destroy(old_entity);
        this->destroy_sigh.publish(*this, entity_id);
        return version;
    }

    entity_type get(const global_entity_type& entity_id) {
        if (!this->global_entities.contains(entity_id)) {
            spdlog::error("Tried to access non-existing Entity {} in Registry", entity_id);
            throw std::runtime_error{"entity-access failed"};
        }
        return this->global_entities[entity_id];
    }

    void swap(global_entity_mixin<underlying_type, global_entity_type>& other) noexcept {
        using std::swap;
        swap(global_entities, other.global_entities);
        swap(create_sigh, other.create_sigh);
        swap(destroy_sigh, other.destroy_sigh);
        underlying_type::swap(other);
    }

private:
    std::unordered_map<global_entity_type, entity_type> global_entities;
    sigh_type create_sigh;
    sigh_type destroy_sigh;
};

} // namespace ecs
} // namespace buildit

#endif // GLOBALENTITYREGISTRY_HPP
