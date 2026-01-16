//
// Created by felix on 11.01.26.
//

#include "modules/module_api.hpp"

using namespace buildit;

class network_module_t final : public modules::api::module_t {
    std::string broadcast_address;
    std::string push_address;
    std::string receive_children_address;
    std::string receive_parent_address;

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

};

extern "C" API_EXPORT std::unique_ptr<modules::api::module_t> create_module() {
    return std::make_unique<network_module_t>();
}