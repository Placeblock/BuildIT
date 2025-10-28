//
// Created by felix on 18.07.25.
//

#ifndef COMPONENT_TYPE_REGISTRY_H
#define COMPONENT_TYPE_REGISTRY_H
#include "simulation/graph.hpp"
#include <entt/entt.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class base_chip_type_t {
protected:
    std::string key;

public:
    virtual void create_chip(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual sim::node_t *get_sim_node(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual void update_chip_graphics(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual ~base_chip_type_t() = default;
};

template<sim::IsSimNode S, typename G>
class chip_type_t : public base_chip_type_t {
    using simulation_type = S;
    using graphics_type = G;

    ~chip_type_t() override = default;

    [[nodiscard]] virtual simulation_type create_chip() const = 0;

    [[nodiscard]] virtual graphics_type get_graphics(simulation_type *sim) const = 0;

public:
    sim::node_t *get_sim_node(entt::registry &reg, const entt::entity &entity) const override;

    void create_chip(entt::registry &reg, const entt::entity &entity) const override;

    void update_chip_graphics(entt::registry &reg, const entt::entity &entity) const override;
};

template<sim::IsSimNode S, typename G>
sim::node_t *chip_type_t<S, G>::
get_sim_node(entt::registry &reg, const entt::entity &entity) const {
    auto &&storage = reg.storage<S>(
        entt::hashed_string(this->key.c_str(), this->key.size()));
    return &storage.get(entity);
}

template<sim::IsSimNode S, typename G>
void chip_type_t<S, G>::create_chip(entt::registry &reg, const entt::entity &entity) const {
    auto &&storage = reg.storage<S>(
        entt::hashed_string(this->key.c_str(), this->key.size()));
    auto iter = storage.push(entity);
    (*iter) = create_chip();

}

template<sim::IsSimNode S, typename G>
void chip_type_t<S, G>::
update_chip_graphics(entt::registry &reg, const entt::entity &entity) const {
    auto &&sim_storage = reg.storage<S>(
        entt::hashed_string(this->key.c_str(), this->key.size()));
    auto &&storage = reg.storage<G>(
        entt::hashed_string(this->key.c_str(), this->key.size()));
    graphics_type graphics = get_graphics(&sim_storage.get(entity));
    storage.patch(entity,
                  [graphics](auto &old_graphics) {
                      old_graphics = graphics;
                  });
}

class chip_type_registry_t {
public:
    void register_chip_type(const std::string &key, std::unique_ptr<base_chip_type_t> &chip_type);

    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<base_chip_type_t> > &
    get_chip_types() const;

    [[nodiscard]] const base_chip_type_t *get_chip_type(const std::string &key) const;

private:
    std::unordered_map<std::string, std::unique_ptr<base_chip_type_t> > chip_types;
};

#endif //COMPONENT_TYPE_REGISTRY_H
