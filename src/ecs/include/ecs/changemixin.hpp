#ifndef HISTORYMIXIN_HPP
#define HISTORYMIXIN_HPP

#include "entt/signal/sigh.hpp"
#include <entt/entity/registry.hpp>
#include <memory>

namespace buildit::ecs {

struct component_change {};

template<typename Type>
struct create_component_change : component_change<Type> {
    Type newValue;
};
template<typename Type>
struct destruct_component_change : component_change<Type> {
    Type oldValue;
};
template<typename Type>
struct update_component_change : component_change<Type> {
    Type oldValue;
    Type newValue;
};

/**
 * @brief Mixin type used to add change support to storage types.
 * 
 * A more sophisticated change-observer remembering old and new elements.
 */
template<typename Type, typename Registry>
class change_mixin : public Type {
    using underlying_type = Type;
    using owner_type = Registry;
    using entity_type = typename underlying_type::entity_type;

    using basic_registry_type = entt::basic_registry<typename owner_type::entity_type,
                                                     typename owner_type::allocator_type>;

    using element_type = underlying_type::element_type;

    using create_sigh_type = entt::sigh<void(owner_type &, create_component_change<element_type> &),
                                        typename underlying_type::allocator_type>;
    using update_sigh_type = entt::sigh<void(owner_type &, update_component_change<element_type> &),
                                        typename underlying_type::allocator_type>;
    using destruct_sigh_type
        = entt::sigh<void(owner_type &, destruct_component_change<element_type> &),
                     typename underlying_type::allocator_type>;
    using underlying_iterator = typename underlying_type::base_type::basic_iterator;

    static_assert(std::is_base_of_v<basic_registry_type, owner_type>, "Invalid registry type");

    [[nodiscard]] auto &owner_or_assert() const noexcept {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        return static_cast<owner_type &>(*owner);
    }

    void publish_create(const entity_type &entt) {
        const auto &reg = this->owner_or_assert();
        element_type element = underlying_type::get(entt);
        const create_component_change<element_type> change{element};
        this->create_sigh.publish(reg, std::move(change));
    }
    void publish_change(const entity_type &entt, const element_type &oldValue) {
        const auto &reg = this->owner_or_assert();
        element_type newValue = underlying_type::get(entt);
        const update_component_change<element_type> change{oldValue, newValue};
        this->update_sigh.publish(reg, std::move(change));
    }
    void publish_destruct(const entity_type &entt) {
        const auto &reg = this->owner_or_assert();
        element_type oldValue = underlying_type::get(entt);
        const destruct_component_change<element_type> change{oldValue};
        this->destruct_sigh.publish(reg, std::move(change));
    }

    void pop(underlying_iterator first, underlying_iterator last) {
        if (auto &reg = this->owner_or_assert(); destruct_sigh.empty()) {
            underlying_type::pop(first, last);
        } else {
            for (; first != last; ++first) {
                const auto entt = *first;
                this->publishDestruct(entt);
                const auto it = underlying_type::find(entt);
                underlying_type::pop(it, it + 1u);
            }
        }
    }

    void pop_all() final {
        if (auto &reg = owner_or_assert(); !destruct_sigh.empty()) {
            for (auto entt : static_cast<typename underlying_type::base_type &>(*this)) {
                if constexpr (underlying_type::storage_policy == entt::deletion_policy::in_place) {
                    if (entt != entt::tombstone) {
                        this->publishDestruct(entt);
                    }
                } else {
                    this->publishDestruct(entt);
                }
            }
        }

        underlying_type::pop_all();
    }

public:
    using change_storage = void; // Tag for checking if a storage has change-support
    using allocator_type = typename underlying_type::allocator_type;

    change_mixin() : change_mixin{allocator_type{}} {}

    explicit change_mixin(const allocator_type &allocator)
        : underlying_type{allocator}
        , owner{}
        , create_sigh{allocator}
        , update_sigh{allocator}
        , destruct_sigh{allocator} {}

    change_mixin(const change_mixin &) = delete;

    change_mixin(change_mixin &&other) noexcept
        : underlying_type{std::move(other)}
        , owner{other.owner}
        , create_sigh{other.create_sigh}
        , update_sigh{other.update_sigh}
        , destruct_sigh{other.destruct_sigh} {}

    change_mixin(change_mixin &&other, const allocator_type &allocator)
        : underlying_type{std::move(other), allocator}
        , owner{other.owner}
        , create_sigh{std::move(other.create_sigh), allocator}
        , update_sigh{std::move(other.update_sigh), allocator}
        , destruct_sigh{std::move(other.destruct_sigh), allocator} {}

    ~change_mixin() override = default;

    change_mixin &operator=(const change_mixin &) = delete;

    change_mixin &operator=(change_mixin &&other) noexcept {
        swap(other);
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given storage.
     * @param other Storage to exchange the content with.
     */
    void swap(change_mixin &other) noexcept {
        using std::swap;
        swap(create_sigh, other.create_sigh);
        swap(update_sigh, other.update_sigh);
        swap(destruct_sigh, other.destruct_sigh);
        underlying_type::swap(other);
    }

    /**
     * @brief Creates a new identifier or recycles a destroyed one.
     * @return A valid identifier.
     */
    auto generate() {
        const auto entt = underlying_type::generate();
        this->publishCreate(entt);
        return entt;
    }

    /**
     * @brief Creates a new identifier or recycles a destroyed one.
     * @param hint Required identifier.
     * @return A valid identifier.
     */
    entity_type generate(const entity_type hint) {
        const auto entt = underlying_type::generate(hint);
        this->publishCreate(entt);
        return entt;
    }

    /**
     * @brief Assigns each element in a range an identifier.
     * @tparam It Type of mutable forward iterator.
     * @param first An iterator to the first element of the range to generate.
     * @param last An iterator past the last element of the range to generate.
     */
    template<typename It>
    void generate(It first, It last) {
        underlying_type::generate(first, last);

        if (auto &reg = this->owner_or_assert(); !this->change.empty()) {
            for (; first != last; ++first) {
                this->publishCreate(*first);
            }
        }
    }

    /**
     * @brief Assigns an entity to a storage and constructs its object.
     * @tparam Args Types of arguments to forward to the underlying storage.
     * @param entt A valid identifier.
     * @param args Parameters to forward to the underlying storage.
     * @return A reference to the newly created object.
     */
    template<typename... Args>
    decltype(auto) emplace(const entity_type entt, Args &&...args) {
        underlying_type::emplace(entt, std::forward<Args>(args)...);
        this->publishCreate(entt);
        return this->get(entt);
    }

    /**
     * @brief Updates the instance assigned to a given entity in-place.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     * @return A reference to the patched instance.
     */
    template<typename... Func>
    decltype(auto) patch(const entity_type entt, Func &&...func) {
        const element_type &oldValue = this->get(entt);
        underlying_type::patch(entt, std::forward<Func>(func)...);
        this->publishChange(entt, oldValue);
        return this->get(entt);
    }

    /**
     * @brief Assigns one or more entities to a storage and constructs their
     * objects from a given instance.
     * @tparam It Type of input iterator.
     * @tparam Args Types of arguments to forward to the underlying storage.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param args Parameters to use to forward to the underlying storage.
     */
    template<typename It, typename... Args>
    void insert(It first, It last, Args &&...args) {
        auto from = underlying_type::size();
        underlying_type::insert(first, last, std::forward<Args>(args)...);

        if (auto &reg = this->owner_or_assert(); !this->change.empty()) {
            // fine as long as insert passes force_back true to try_emplace
            for (const auto to = underlying_type::size(); from != to; ++from) {
                this->publishCreate(underlying_type::operator[](from));
            }
        }
    }

    [[nodiscard]] auto on_create_change() noexcept {
        return entt::sink{create_sigh};
    }
    [[nodiscard]] auto on_update_change() noexcept {
        return entt::sink{update_sigh};
    }
    [[nodiscard]] auto on_destruct_change() noexcept {
        return entt::sink{destruct_sigh};
    }

private:
    basic_registry_type *owner;
    create_sigh_type create_sigh;
    update_sigh_type update_sigh;
    destruct_sigh_type destruct_sigh;
};

template<typename Type>
concept is_change_storage = requires() {
    { Type::change_storage };
};

/**
 * @brief Adds change support to a registry.
 * Forwards events from the change storage mixin
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

#endif // HISTORYMIXIN_HPP
