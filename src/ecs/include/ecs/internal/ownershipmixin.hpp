//
// Created by felix on 17.07.25.
//

#ifndef OWNERSHIPMIXIN_HPP
#define OWNERSHIPMIXIN_HPP

#include "../../../../common/include/common/player.hpp"
#include "../../../../lib/entt/src/entt/entt.hpp"
#include <spdlog/spdlog.h>

namespace buildit::ecs {

class ownership_storage_mixin;

/**
 * @brief Adds ownership support to a registry.
 * Each entity then belongs to a player
 */
template<typename Type>
class ownership_registry_mixin : public Type {
    using underlying_type = Type;
    using global_entity_type = typename underlying_type::global_entity_type;
    using entity_type = typename underlying_type::entity_type;
    using version_type = typename underlying_type::version_type;
    using allocator_type = typename underlying_type::allocator_type;
    using size_type = typename underlying_type::size_type;

    friend ownership_storage_mixin;
    void set_ownership(const global_entity_type &global_entt, player_id player) {
        this->ownership[global_entt] = player;
    }

public:
    using Type::get;

    /*! @brief Default constructor. */
    ownership_registry_mixin() : ownership_registry_mixin{allocator_type{}} {}

    /**
     * @brief Constructs an empty registry with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit ownership_registry_mixin(const allocator_type &allocator)
        : ownership_registry_mixin{0u, allocator} {}

    /**
     * @brief Allocates enough memory upon construction to store `count` pools.
     * @param count The number of pools to allocate memory for.
     * @param allocator The allocator to use.
     */
    explicit ownership_registry_mixin(const size_type count,
                                      const allocator_type &allocator = allocator_type{})
        : underlying_type(count, allocator), ownership{allocator} {}

    /*! @brief Default copy constructor, deleted on purpose. */
    ownership_registry_mixin(const ownership_registry_mixin &) = delete;

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    ownership_registry_mixin(ownership_registry_mixin &&other) noexcept
        : underlying_type{std::move(other)}, ownership(std::move(other.ownership)) {}

    /*! @brief Default destructor. */
    ~ownership_registry_mixin() = default;

    /**
     * @brief Default copy assignment operator, deleted on purpose.
     * @return This mixin.
     */
    ownership_registry_mixin &operator=(const ownership_registry_mixin &) = delete;

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This registry.
     */
    ownership_registry_mixin &operator=(ownership_registry_mixin &&other) noexcept {
        swap(other);
        return *this;
    }

    entity_type create(const player_id player, const global_entity_type &global_entt) {
        const entity_type &new_entity = Type::create(global_entt);
        this->ownership[global_entt] = player;
        return new_entity;
    }

    [[nodiscard]] entity_type get_or_create(const player_id player,
                                            const global_entity_type &global_entt) {
        if (this->global_entities.contains(global_entt)) {
            return this->entity(global_entt);
        }
        return this->create(player, global_entt);
    }

    version_type destroy(const player_id player, const global_entity_type &global_entt) {
        const auto version = Type::destroy(global_entt);
        this->ownership[global_entt] = player;
        return version;
    }

    [[nodiscard]] player_id owner(const global_entity_type &global_entt) {
        if (!this->ownership.contains(global_entt)) {
            spdlog::error("Tried to access non-existing Entity {} in Ownership Map", global_entt);
            throw std::runtime_error{"ownership-access failed"};
        }
        return this->ownership[global_entt];
    }

    void swap(ownership_registry_mixin &other) noexcept {
        using std::swap;
        swap(ownership, other.ownership);
        underlying_type::swap(other);
    }

private:
    entt::dense_map<global_entity_type, player_id> ownership;
};

} // namespace buildit::ecs

#endif //OWNERSHIPMIXIN_HPP
