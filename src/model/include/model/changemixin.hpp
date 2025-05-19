#ifndef HISTORYMIXIN_HPP
#define HISTORYMIXIN_HPP

#include "entt/signal/sigh.hpp"
#include <entt/entity/registry.hpp>
#include <memory>
namespace buildit {
namespace ecs {

template<typename Type>
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

private:
    void publishCreate(const entity_type &entt) {
        const auto &reg = this->owner_or_assert();
        element_type element = underlying_type::get(entt);
        const create_component_change<element_type> change{element};
        create_sigh.publish(reg, std::move(change));
    }
    void publishChange(const entity_type &entt, const element_type &oldValue) {
        const auto &reg = this->owner_or_assert();
        element_type newValue = underlying_type::get(entt);
        const update_component_change<element_type> change{oldValue, newValue};
        update_sigh.publish(reg, std::move(change));
    }
    void publishDestruct(const entity_type &entt) {
        const auto &reg = this->owner_or_assert();
        element_type oldValue = underlying_type::get(entt);
        const destruct_component_change<element_type> change{oldValue};
        destruct_sigh.publish(reg, std::move(change));
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

} // namespace ecs
} // namespace buildit
#endif // HISTORYMIXIN_HPP
