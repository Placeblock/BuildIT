//
// Created by felix on 08.04.25.
//

#ifndef SERALIZATION_HPP
#define SERALIZATION_HPP

#include <cereal/cereal.hpp>
#include <entt/entt.hpp>

namespace Models {
    template <class Archive>
    class Serialization {
        Archive &archive;
        
    public:
        // ReSharper disable once CppMemberFunctionMayBeConst Non-Const needed by EnTT
        void operator()(entt::entity entity) {
            printf("Serializing Entity\n");
            archive(cereal::make_nvp("entity", entity));
        }
        
        // ReSharper disable once CppMemberFunctionMayBeConst Non-Const needed by EnTT
        void operator()(std::underlying_type_t<entt::entity> size) {
            printf("Serializing Size\n");
            archive(cereal::make_nvp("size", size));
        }

        template <typename T>
        void operator()(const T &component) {
            printf("Serializing Component\n");
            archive(cereal::make_nvp("component", component));
        }

        explicit Serialization(Archive &archive) : archive(archive) {}
    };

} // Models

#endif //SERALIZATION_HPP
