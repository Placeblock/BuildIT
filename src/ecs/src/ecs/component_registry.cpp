//
// Created by felix on 18.07.25.
//

#include "ecs/component_registry.hpp"

void component_registry::register_component(std::unique_ptr<component> &component) {
    if (this->components.contains(component->key)) {
        throw std::runtime_error("component '" + component->name + "' already registered");
    }
    this->components[component->key] = std::move(component);
}

const std::map<std::string, std::unique_ptr<component>> &component_registry::get_components() const {
    return this->components;
}
const component &component_registry::get_component(const std::string &key) const {
    return *this->components.at(key);
}
void component_registry::create_component(entt::registry &reg, const std::string &key) const {
    if (!this->components.contains(key)) {
        throw std::runtime_error("component '" + key + "' not registered");
    }
    this->components.at(key)->create(reg);
}