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

namespace buildit::ecs {
class base_chip_type_t {
    constexpr static std::string SIMULATION_STORAGE_SUFFIX = "_simulation";
    constexpr static std::string GRAPHICS_STORAGE_SUFFIX = "_graphics";

protected:
    std::string key;
    uint8_t width, height;
    entt::hashed_string simulation_storage;
    entt::hashed_string graphics_storage;

public:
    explicit base_chip_type_t(const std::string &key, uint8_t width, uint8_t height);

    virtual void create_chip(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual sim::node_t *get_sim_node(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual void update_chip_graphics(entt::registry &reg, const entt::entity &entity) const = 0;

    virtual ~base_chip_type_t() = default;
};

template<sim::IsSimNode S, typename G>
class chip_type_t : public base_chip_type_t {
protected:
    using simulation_type = S;
    using graphics_type = G;

public:
    explicit chip_type_t(const std::string &key, uint8_t width, uint8_t height);

    ~chip_type_t() override = default;

    [[nodiscard]] virtual simulation_type create_chip() const = 0;

    [[nodiscard]] virtual graphics_type get_graphics(simulation_type *sim) const = 0;

    sim::node_t *get_sim_node(entt::registry &reg, const entt::entity &entity) const override;

    void create_chip(entt::registry &reg, const entt::entity &entity) const override;

    void update_chip_graphics(entt::registry &reg, const entt::entity &entity) const override;
};

template<sim::IsSimNode S, typename G>
chip_type_t<
    S, G>::chip_type_t(const std::string &key,
                       const uint8_t width,
                       const uint8_t height) : base_chip_type_t(key, width, height) {
}

template<sim::IsSimNode S, typename G>
sim::node_t *chip_type_t<S, G>::
get_sim_node(entt::registry &reg, const entt::entity &entity) const {
    auto &&storage = reg.storage<S>(this->simulation_storage);
    return &storage.get(entity);
}

template<sim::IsSimNode S, typename G>
void chip_type_t<S, G>::create_chip(entt::registry &reg, const entt::entity &entity) const {
    auto &storage = reg.storage<S>(this->simulation_storage);
    storage.emplace(entity, create_chip());
}

template<sim::IsSimNode S, typename G>
void chip_type_t<S, G>::
update_chip_graphics(entt::registry &reg, const entt::entity &entity) const {
    auto &&sim_storage = reg.storage<S>(
        entt::hashed_string(this->simulation_storage));
    auto &&storage = reg.storage<G>(
        entt::hashed_string(this->graphics_storage));
    graphics_type graphics = get_graphics(&sim_storage.get(entity));
    if (storage.contains(entity)) {
        storage.patch(entity,
                      [graphics](auto &old_graphics) {
                          old_graphics = graphics;
                      });
    } else {
        storage.emplace(entity, graphics);
    }
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
}

#endif //COMPONENT_TYPE_REGISTRY_H
