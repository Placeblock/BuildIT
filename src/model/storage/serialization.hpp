//
// Created by felix on 08.04.25.
//

#ifndef SERALIZATION_HPP
#define SERALIZATION_HPP

#include <cereal/cereal.hpp>
#include <entt/entt.hpp>

namespace Models {
    template <class Archive, typename Registry>
    class Serialization {
        static_assert(!std::is_const_v<Registry>, "Non-const registry type required");
        using traits_type = entt::entt_traits<typename Registry::entity_type>;
        
        Archive &archive;
        
    public:
        using registry_type = Registry;
        using entity_type = typename registry_type::entity_type;

        void serialize(registry_type &registry) {
            const auto *storage = registry->template entt::storage<Type>(id);
            const auto entities = registry.view<entt::entity>();
            cereal::size_type entityAmount = entities.size();
            archive(cereal::make_size_tag(entityAmount));
            for (auto entity : entities) {
                archive(entity);
            }
        }

        explicit Serialization(Archive &archive) : archive(archive) {}
    };

} // Models

#endif //SERALIZATION_HPP
