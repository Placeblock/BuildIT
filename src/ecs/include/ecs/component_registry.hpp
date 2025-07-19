//
// Created by felix on 18.07.25.
//

#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H
#include "entt/entt.hpp"
#include <map>
#include <string>

struct component {
    virtual void create(entt::registry& reg) const = 0;
    virtual ~component() = default;

    std::string key;
    std::string name;
};

class component_registry {
public:
    void register_component(std::unique_ptr<component>& component);
    [[nodiscard]] const std::map<std::string, std::unique_ptr<component>>& get_components() const;
    [[nodiscard]] const component& get_component(const std::string& key) const;
    void create_component(entt::registry& reg, const std::string& key) const;

private:
    std::map<std::string, std::unique_ptr<component>> components;
};

#endif //COMPONENT_REGISTRY_H
