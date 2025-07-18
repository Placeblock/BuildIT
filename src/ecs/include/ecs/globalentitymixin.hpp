#ifndef GLOBALENTITYREGISTRY_HPP
#define GLOBALENTITYREGISTRY_HPP

#include <spdlog/spdlog.h>

namespace buildit::ecs {

/**
 * @brief Adds global-entity-id support to a registry.
 *
 * Provides methods to create get and delete entities using the global entity id.
 */
template<typename Type, typename GlobalEntity>
class global_entity_mixin : public Type {
    using global_entity_type = GlobalEntity;
    using underlying_type = Type;
    using entity_type = typename underlying_type::entity_type;
    using version_type = typename underlying_type::version_type;
    using allocator_type = typename underlying_type::allocator_type;
    using size_type = typename underlying_type::size_type;

    using sigh_type = entt::sigh<void(underlying_type &, const GlobalEntity &),
                                 typename underlying_type::allocator_type>;

public:
    using Type::get;

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
    explicit global_entity_mixin(const size_type count,
                                 const allocator_type &allocator = allocator_type{})
        : underlying_type(count, allocator)
        , global_entities{allocator}
        , entities{allocator}
        , create_sigh{allocator}
        , destroy_sigh{allocator} {}

    /*! @brief Default copy constructor, deleted on purpose. */
    global_entity_mixin(const global_entity_mixin &) = delete;

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    global_entity_mixin(global_entity_mixin &&other) noexcept
        : underlying_type{std::move(other)}
        , global_entities{std::move(other.global_entities)}
        , entities{std::move(other.entities)}
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

    entity_type create(const global_entity_type &global_entt) {
        if (this->global_entities.contains(global_entt)) {
            spdlog::error("Tried to create already existing Entity {} in Registry", global_entt);
            throw std::runtime_error{"entity-creation failed"};
        }
        const entity_type &new_entity = Type::create();
        this->global_entities[global_entt] = new_entity;
        this->entities[new_entity] = global_entt;
        this->create_sigh.publish(*this, global_entt);
        return new_entity;
    }

    [[nodiscard]] entity_type get_or_create(const global_entity_type &global_entt) {
        if (this->global_entities.contains(global_entt)) {
            return this->entity(global_entt);
        }
        return this->create(global_entt);
    }

    version_type destroy(const global_entity_type &global_entt) {
        if (!this->global_entities.contains(global_entt)) {
            spdlog::error("Tried to remove non-existing Entity {} in Registry", global_entt);
            throw std::runtime_error{"entity-destruction failed"};
        }
        const entity_type old_entity = this->global_entities[global_entt];
        this->global_entities.erase(global_entt);
        this->entities.erase(old_entity);
        const auto version = this->destroy(old_entity);
        this->destroy_sigh.publish(*this, global_entt);
        return version;
    }

    [[nodiscard]] entity_type entity(const global_entity_type &global_entt) {
        if (!this->global_entities.contains(global_entt)) {
            spdlog::error("Tried to access non-existing Entity {} in Registry", global_entt);
            throw std::runtime_error{"entity-access failed"};
        }
        return this->global_entities[global_entt];
    }
    [[nodiscard]] global_entity_type entity(const entity_type &entt) {
        if (!this->entities.contains(entt)) {
            spdlog::error("Tried to access non-existing Entity {} in Registry", entt);
            throw std::runtime_error{"entity-access failed"};
        }
        return this->entities[entt];
    }

    template<typename... T>
    [[nodiscard]] decltype(auto) get([[maybe_unused]] const global_entity_type global_entt) {
        const entity_type entt = this->entity(global_entt);
        return Type::template get<T...>(entt);
    }

    void swap(global_entity_mixin &other) noexcept {
        using std::swap;
        swap(global_entities, other.global_entities);
        swap(entities, other.entities);
        swap(create_sigh, other.create_sigh);
        swap(destroy_sigh, other.destroy_sigh);
        underlying_type::swap(other);
    }

private:
    entt::dense_map<global_entity_type, entity_type> global_entities;
    entt::dense_map<entity_type, global_entity_type> entities;
    sigh_type create_sigh;
    sigh_type destroy_sigh;
};

} // namespace buildit::ecs

#endif // GLOBALENTITYREGISTRY_HPP
