//
// Created by felix on 19.05.25.
//

#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP
#include "model/components.hpp"
#include <bitsery/adapter/stream.h>
#include <bitsery/bitsery.h>
#include <spdlog/spdlog.h>

namespace buildit::ecs::serialization {

using output_adapter = bitsery::OutputStreamAdapter;
using input_adapter = bitsery::InputStreamAdapter;
using deserializer = bitsery::Deserializer<input_adapter>;
using serializer = bitsery::Serializer<output_adapter>;

template<typename Type>
concept has_key = requires(Type t) {
    { Type::Key } -> std::same_as<const std::string &>;
};

class registry_serializer {
    using global_entity_component_type = registry::global_entity_component_type;

public:
    explicit registry_serializer(registry *reg);

    template<has_key Type>
    const registry_serializer &serialize(serializer &serializer) const {
        using storage_type = registry::storage_for_type<Type>;

        std::string key = Type::Key;

        const storage_type *storage = this->reg->storage<Type>();
        if (storage == nullptr) {
            spdlog::error("Tried to serialize Component {} without storage.", key);
        }

        size_t size = storage->size();
        serializer.text1b(key, 64);
        serializer.value8b(size);

        if constexpr (registry::template storage_for_type<Type>::storage_policy
                      == entt::deletion_policy::in_place) {
            for (auto elem : storage->reach()) {
                if (const entity entt = std::get<0>(elem); entt != entt::tombstone) {
                    const global_entity_component_type global_entt_component
                        = this->reg->get<global_entity_component_type>(entt);
                    serializer.value<sizeof(global_entity)>(global_entt_component.id);
                    serializer.object<Type>(std::get<1>(elem));
                }
            }
        } else {
            for (auto elem : storage->reach()) {
                const entity entt = std::get<0>(elem);
                const global_entity_component_type global_entt_component
                    = this->reg->get<global_entity_component_type>(entt);
                serializer.value<sizeof(global_entity)>(global_entt_component.id);
                serializer.object<Type>(std::get<1>(elem));
            }
        }
        return *this;
    }

private:
    const registry *reg;
};

class basic_component_loader {
public:
    virtual ~basic_component_loader();
    virtual void load(deserializer &deser, registry &reg, entity entt) const = 0;
};

template<typename Type>
class component_loader final : public basic_component_loader {
public:
    void load(deserializer &deser, registry &reg, entity entt) const override {
        Type data;
        deser.object<Type>(&data);
        reg.emplace<Type>(entt, data);
    }
};

class registry_deserializer {
public:
    explicit registry_deserializer(registry *reg);

    void deserialize(deserializer &deserializer) const;

private:
    std::unordered_map<std::string, basic_component_loader *> loaders;
    registry *reg;
};
} // namespace buildit::ecs::serialization

#endif //SNAPSHOT_HPP
