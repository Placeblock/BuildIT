//
// Created by felix on 08.04.25.
//

#ifndef SERALIZATION_HPP
#define SERALIZATION_HPP

#include <cereal/cereal.hpp>
#include <entt/entt.hpp>

namespace Models {
    template<typename Storage>
    struct Entities {
        const Storage *storage;

        template<class Archive>
        void serialize(Archive &archive) {
            cereal::size_type entityAmount = storage->size();
            archive(cereal::make_size_tag(entityAmount));
            for (auto first = storage->rbegin(), last = storage->rend(); first != last; ++first) {
                archive(*first);
            }
        };
    };

    template<typename Component>
    struct SerializedComponent {
        entt::entity &entity;
        Component &component;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(entity), CEREAL_NVP(component));
        }
    };

    template<typename Storage, typename Type>
    struct Components {
        const Storage *storage;

        template<class Archive>
        void serialize(Archive &archive) {
            cereal::size_type componentsAmount = storage->size();
            archive(cereal::make_size_tag(componentsAmount));
            for(auto elem: storage->reach()) {
                entt::entity entity = std::get<0>(elem);
                Type componentData = std::get<1>(elem);
                SerializedComponent<Type> component{entity, componentData};
                archive(CEREAL_NVP(component));
            }
        }
    };

    template<typename Registry>
    class Serialization {
        static_assert(!std::is_const_v<Registry>, "Non-const registry type required");
        using traits_type = entt::entt_traits<typename Registry::entity_type>;

    public:
        using registry_type = Registry;
        using entity_type = typename registry_type::entity_type;
        using allocator_type = typename registry_type::allocator_type;


        template<typename Type, typename Archive>
        const Serialization &serialize(Archive &archive,
                                       const entt::id_type id = entt::type_hash<Type>::value()) const {
            using storage_type = typename entt::storage_type<Type>::type;
            const auto *storage = reg->template storage<Type>(id);
            if (storage) {
                if constexpr (std::is_same_v<Type, entity_type>) {
                    Entities<storage_type> entities{storage};
                    archive(CEREAL_NVP(entities));
                } else {
                    Components<storage_type, Type> components{storage};
                    archive(CEREAL_NVP(components));
                }
            }
            return *this;
        }

        explicit Serialization(const registry_type &source) noexcept : reg{&source} {
        }

    private:
        const registry_type *reg;
    };
} // Models

#endif //SERALIZATION_HPP
