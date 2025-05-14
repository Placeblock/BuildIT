#ifndef BASICCHANGEMIXIN_HPP
#define BASICCHANGEMIXIN_HPP

#include "model/components.hpp"

#include "worldhistory.hpp"

/**
 * @brief Mixin type used to add change-detection support to storage types
 * @tparam Type Underlying storage type.
 */
template<typename Type>
class basic_component_change_mixin final : public Type {
    using underlying_type = Type;
    using owner_type = BuildIT::Registry;
    using sigh_type = entt::sigh<void(owner_type &, std::unique_ptr<Change>),
                                 typename underlying_type::allocator_type>;
    using underlying_iterator = typename underlying_type::base_type::basic_iterator;

    [[nodiscard]] auto &owner_or_assert() const noexcept {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        return static_cast<owner_type &>(*owner);
    }

public:
    /**
     * @brief Assigns an entity to a storage and constructs its object.
     * @tparam Args Types of arguments to forward to the underlying storage.
     * @param entity A valid identifier.
     * @param args Parameters to forward to the underlying storage.
     * @return A reference to the newly created object.
     */
    template<typename... Args>
    decltype(auto) emplace(const BuildIT::Entity entity, Args &&...args) {
        underlying_type::emplace(entity, std::forward<Args>(args)...);
        auto &registry = owner_or_assert();
        GlobalEntityId entityId = registry.template get<GlobalEntityId>(entity);
        underlying_type &data = this->get(entity);
        std::unique_ptr<Change> change
            = std::make_unique<ComponentEmplaceChange<underlying_type>>(entityId, data);
        this->sigh.publish(registry, std::move(change));
        return this->get(entity);
    }

    [[nodiscard]] auto on_change() noexcept {
        return entt::sink{this->sigh};
    }

private:
    BuildIT::Registry *owner;
    sigh_type sigh;
}

#endif // BASICCHANGEMIXIN_HPP
