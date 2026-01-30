//
// Created by felix on 11.01.26.
//

#include "buildit/network/communication.hpp"
#include "buildit/network/network.hpp"
#include "ecs_history/history.hpp"
#include "ecs_history/gather_strategy/reactive/reactive_gather_strategy.hpp"
#include "modules/module_api.hpp"

using namespace buildit;

class network_module_t final : public modules::api::module_t {
    zmq::context_t ctx{};

    std::string broadcast_bind_address;
    std::string broadcast_connect_address;
    std::string push_connect_address;
    std::string push_bind_address;
    std::string communication_bind_address;
    std::string communication_connect_address;

    std::unique_ptr<registry_node_t> node;
    std::thread send_thread;

    std::unique_ptr<communication_child_t> communication_child;
    std::unique_ptr<communication_parent_t> communication_parent;

public:
    [[nodiscard]] std::string get_name() const override {
        return "network";
    }

    [[nodiscard]] std::string get_description() const override {
        return "Network module which handles multiplayer synchronization";
    }

    void init(ini::IniFile &config) override {
        ini::IniSection general = config["general"];
        this->broadcast_bind_address = general["broadcast-bind-address"].as<std::string>();
        this->push_connect_address = general["push-connect-address"].as<std::string>();
        this->push_bind_address = general["push-bind-address"].as<std::string>();
        this->broadcast_connect_address = general["broadcast-connect-address"].as<std::string>();
        this->communication_bind_address = general["communication-bind-address"].as<std::string>();
        this->communication_connect_address = general["communication-connect-address"].as<
            std::string>();
    }

    void run(modules::api::locked_registry_t &reg) override {
        this->node = std::make_unique<registry_node_t>(this->ctx,
                                                       reg,
                                                       this->broadcast_bind_address,
                                                       this->push_connect_address,
                                                       this->broadcast_connect_address,
                                                       this->push_bind_address);
        this->send_thread = std::thread([this, &reg]() {
            this->send_changes(reg);
        });
        this->send_thread.detach();
        if (!this->communication_connect_address.empty()) {
            this->communication_child = std::make_unique<communication_child_t>(
                this->ctx,
                reg,
                this->communication_connect_address);
        }
        if (!this->communication_bind_address.empty()) {
            this->communication_parent = std::make_unique<communication_parent_t>(
                this->ctx,
                reg,
                this->communication_bind_address);
        }
    }

    [[noreturn]] void send_changes(modules::api::locked_registry_t &reg) const {
        spdlog::info("sending changes now!");

        auto &version_handler = reg.handle.ctx().get<ecs_history::entity_version_handler_t>();
        auto &gather_strategy = reg.handle.ctx().get<std::shared_ptr<
            ecs_history::gather_strategy_t> >();
        ecs_history::commit_id_generator_t id_generator;

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            std::shared_lock lock(reg.mutex);

            std::unique_ptr<ecs_history::commit_t> commit = ecs_history::create_commit(
                *gather_strategy,
                version_handler);
            if (commit->entity_versions.empty()) {
                continue;
            }

            std::stringstream oss{};
            cereal::PortableBinaryOutputArchive archive(oss);
            ecs_history::serialization::serialize_commit(archive, *commit);
            auto commit_string = oss.str();
            auto commit_data = zmq::const_buffer{commit_string.data(), commit_string.size()};

            const ecs_history::commit_id id = id_generator.next();
            ecs_history::commit_id base_id = reg.history.add_commit(id, commit);

            std::stringstream header_oss{};
            cereal::PortableBinaryOutputArchive header_archive(header_oss);
            header_archive(base_id);
            header_archive(id);
            auto header_string = header_oss.str();
            zmq::const_buffer header_data{header_string.data(), header_string.size()};

            commit_message_t commit_message{base_id, id, header_data, commit_data};
            spdlog::debug("sending commit");
            if (this->node->broadcast_commits) {
                registry_node_t::send_commit_message(this->node->broadcast_socket,
                                                     this->node->broadcast_mutex,
                                                     commit_message);
            }
            if (this->node->push_commits) {
                registry_node_t::send_commit_message(this->node->push_socket,
                                                     this->node->push_server_mutex,
                                                     commit_message);
            }
        }
    }

};

extern "C" API_EXPORT std::unique_ptr<modules::api::module_t> create_module() {
    return std::make_unique<network_module_t>();
}