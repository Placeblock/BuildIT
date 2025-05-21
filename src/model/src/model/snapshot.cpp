//
// Created by felix on 23.04.25.
//

#include "model/snapshot.hpp"
#include <bitsery/traits/string.h>

using namespace buildit::ecs::serialization;

registry_serializer::registry_serializer(registry *reg) : reg(reg) {}

registry_deserializer::registry_deserializer(registry *reg) : reg(reg) {}

void registry_deserializer::deserialize(deserializer &deserializer) const {
    while (!deserializer.adapter().isCompletedSuccessfully()) {
        std::string key;
        deserializer.text1b(key, 64);

        if (!loaders.contains(key)) {
            spdlog::error("Could not load unknown component {}. Plugin missing?", key);
            throw std::runtime_error{"unknown component error"};
        }

        size_t size;
        deserializer.value8b(size);

        for (int i = 0; i < size; ++i) {
            global_entity global_entt;
            deserializer.value<sizeof(global_entity)>(global_entt);
            const auto &entt = this->reg->get_or_create(global_entt);

            this->loaders.at(key)->load(deserializer, *this->reg, entt);
        }
    }
}
