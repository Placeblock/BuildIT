//
// Created by felix on 19.05.25.
//

#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP
#include "model/components.hpp"
#include <bitsery/adapter/stream.h>
#include <bitsery/bitsery.h>
#include <spdlog/spdlog.h>

namespace buildit::ecs {

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
    explicit registry_serializer(const registry *reg);

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

        if constexpr (registry::storage_for_type<Type>::storage_policy
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
    virtual ~basic_component_loader() {};
    virtual void load(deserializer &deser, registry &reg, entity entt) const = 0;
};

template<typename Type>
class component_loader final : public basic_component_loader {
public:
    void load(deserializer &deser, registry &reg, entity entt) const override {
        Type data;
        deser.object(data);
        reg.emplace<Type>(entt, data);
    }
};

class loader_registry {
    std::unordered_map<std::string, std::unique_ptr<basic_component_loader>> loaders;

public:
    basic_component_loader *operator[](const std::string &key) const;
    bool contains(const std::string &key) const;
    template<has_key Type>
    void register_loader() {
        std::unique_ptr<basic_component_loader> loader = std::make_unique<component_loader<Type>>();
        this->register_loader(Type::Key, loader);
    }
    void register_loader(std::string key, std::unique_ptr<basic_component_loader> &loader);
};

class registry_deserializer {
public:
    registry_deserializer(registry *reg, loader_registry *loader_reg);

    void deserialize(deserializer &deserializer) const;

private:
    registry *reg;
    loader_registry *loader_reg;
};
} // namespace buildit::ecs

#endif //SNAPSHOT_HPP
