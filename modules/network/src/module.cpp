//
// Created by felix on 11.01.26.
//

#include "buildit/network/network.hpp"
#include "ecs_history/gather_strategy/reactive/reactive_gather_strategy.hpp"
#include "modules/module_api.hpp"

using namespace buildit;

class network_module_t final : public modules::api::module_t {
    std::string broadcast_address;
    std::string push_address;
    std::string receive_children_address;
    std::string receive_parent_address;

    std::unique_ptr<registry_node_t> node;
    std::unique_ptr<ecs_history::reactive_gather_strategy> gather_strategy;

public:
    [[nodiscard]] std::string get_name() const override {
        return "network";
    }

    [[nodiscard]] std::string get_description() const override {
        return "Network module which handles multiplayer synchronization";
    }

    void init(ini::IniFile &config) override {
        ini::IniSection general = config["general"];
        this->broadcast_address = general["broadcast-address"].as<std::string>();
        this->push_address = general["push-address"].as<std::string>();
        this->receive_children_address = general["receive-children-address"].as<std::string>();
        this->receive_parent_address = general["receive-parent-address"].as<std::string>();
    }

    void run(modules::api::locked_registry_t &reg) override {
        this->gather_strategy = std::make_unique<ecs_history::reactive_gather_strategy>(reg.handle);
        gather_strategy->record_changes<entt::entity>();
        gather_strategy->record_changes<bounding_box_t>();
        gather_strategy->record_changes<gate_t>();
        this->node = std::make_unique<registry_node_t>(reg,
                                                       *this->gather_strategy,
                                                       this->broadcast_address,
                                                       this->push_address,
                                                       this->receive_parent_address,
                                                       this->receive_children_address);
    }

};

extern "C" API_EXPORT std::unique_ptr<modules::api::module_t> create_module() {
    return std::make_unique<network_module_t>();
}