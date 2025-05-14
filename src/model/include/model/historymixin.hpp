#ifndef HISTORYMIXIN_HPP
#define HISTORYMIXIN_HPP

#include "entt/signal/sigh.hpp"
#include <entt/entity/registry.hpp>
#include <memory>
namespace buildit {
namespace ecs {

template<typename Type>
struct Change {};

template<typename Type>
struct CreateChange : Change<Type> {
    Type newValue;
};
template<typename Type>
struct DestructChange : Change<Type> {
    Type oldValue;
};
template<typename Type>
struct UpdateChange : Change<Type> {
    Type oldValue;
    Type newValue;
};

template<typename Type, typename Registry>
class change_mixin : public Type {
    using underlying_type = Type;
    using owner_type = Registry;

    using basic_registry_type = entt::basic_registry<typename owner_type::entity_type,
                                                     typename owner_type::allocator_type>;

    using element_type = underlying_type::element_type;

    using sigh_type = entt::sigh<void(owner_type &, std::unique_ptr<Change<element_type>> &),
                                 typename underlying_type::allocator_type>;

    static_assert(std::is_base_of_v<basic_registry_type, owner_type>, "Invalid registry type");

    [[nodiscard]] auto &owner_or_assert() const noexcept {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        return static_cast<owner_type &>(*owner);
    }

public:
    using allocator_type = typename underlying_type::allocator_type;

    change_mixin() : change_mixin{allocator_type{}} {}

    explicit change_mixin(const allocator_type &allocator)
        : underlying_type{allocator}, owner{}, change{allocator} {}

    change_mixin(const change_mixin &) = delete;

    change_mixin(change_mixin &&other) noexcept
        : underlying_type{std::move(other)}, owner{other.owner}, change{std::move(other.change)} {}

    change_mixin(change_mixin &&other, const allocator_type &allocator)
        : underlying_type{std::move(other), allocator}
        , owner{other.owner}
        , change{std::move(other.change), allocator} {}

    ~change_mixin() override = default;

    change_mixin &operator=(const change_mixin &) = delete;

    change_mixin &operator=(change_mixin &&other) noexcept {
        swap(other);
        return *this;
    }

    void swap(change_mixin &other) noexcept {
        using std::swap;
        swap(change, other.change);
        underlying_type::swap(other);
    }

    [[nodiscard]] auto on_change() noexcept {
        return entt::sink{change};
    }

private:
    basic_registry_type *owner;
    sigh_type change;
};

} // namespace ecs
} // namespace buildit
#endif // HISTORYMIXIN_HPP
