//
// Created by felix on 23.04.25.
//

#include "ecs/internal/snapshot.hpp"
#include <bitsery/traits/string.h>

using namespace buildit::ecs;

registry_serializer::registry_serializer(const registry *reg) : reg(reg) {
}

basic_component_loader *loader_registry::operator[](const std::string &key) const {
    return this->loaders.at(key).get();
}

bool loader_registry::contains(const std::string &key) const {
    return this->loaders.contains(key);
}

void loader_registry::register_loader(std::string key,
                                      std::unique_ptr<basic_component_loader> &loader) {
    if (this->loaders.contains(key)) {
        spdlog::error("Tried to register loader for already registered component '{}'! Was a "
                      "plugin loaded twice?",
                      key);
        throw std::runtime_error{"duplicate component loader error"};
    }
    this->loaders[key] = std::move(loader);
}

registry_deserializer::registry_deserializer(registry *reg, loader_registry *loader_reg)
    : reg(reg), loader_reg(loader_reg) {
}

void registry_deserializer::deserialize(deserializer &deserializer) const {
    while (!deserializer.adapter().isCompletedSuccessfully()) {
        std::string key;
        deserializer.text1b(key, 64);

        if (!this->loader_reg->contains(key)) {
            spdlog::error("Could not load unknown component '{}'! Plugin missing?", key);
            throw std::runtime_error{"unknown component key error"};
        }

        size_t size;
        deserializer.value8b(size);

        for (int i = 0; i < size; ++i) {
            global_entity_t global_entt;
            deserializer.value<sizeof(global_entity_t)>(global_entt);
            const auto &entt = this->reg->get_or_create(global_entt);

            (*this->loader_reg)[key]->load(deserializer, *this->reg, entt);
        }
    }
}
