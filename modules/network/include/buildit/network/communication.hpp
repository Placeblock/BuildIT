//
// Created by felix on 1/18/26.
//

#ifndef BUILDIT_COMMUNICATION_HPP
#define BUILDIT_COMMUNICATION_HPP
#include "protocol.hpp"
#include "ecs_history/serialization/serialization.hpp"
#include "modules/module_api.hpp"
#include <zmq.hpp>
#include <spdlog/spdlog.h>

#define MAX_CATCHUP_SIZE 1000000
#define MIN_CATCHUP_COMMITS 1
#define MAX_CATCHUP_COMMITS 10

class communication_child_t {
    zmq::socket_t socket;
    buildit::modules::api::locked_registry_t &registry;
    std::thread thread;

public:
    explicit communication_child_t(zmq::context_t &ctx,
                                   buildit::modules::api::locked_registry_t &registry,
                                   const std::string &communication_address)
        : socket{ctx, zmq::socket_type::client}, registry(registry) {
        spdlog::info("communication-channel started (child)");
        this->socket.connect("tcp://" + communication_address);

        this->thread = std::thread{&communication_child_t::request_registry, this};
    }

    void request_registry() {
        spdlog::info("requesting registry");
        const auto [string, buffer] = get_buffer([](cereal::PortableBinaryOutputArchive &archive) {
            archive_header<message_types::request_registry>(archive);
        });
        this->socket.send(buffer, zmq::send_flags::dontwait);

        zmq::message_t recv_message;
        if (const zmq::recv_result_t result = this->socket.recv(recv_message); !result) {
            throw std::runtime_error("failed to receive message");
        }

        get_archive(recv_message,
                    [this](cereal::PortableBinaryInputArchive &archive) {
                        deserialize_header<message_types::registry>(archive);
                        std::lock_guard lock(this->registry.mutex);
                        const auto &gather_strategy = this->registry.handle.ctx().get<
                            std::shared_ptr<ecs_history::gather_strategy_t> >();
                        const std::chrono::steady_clock::time_point begin =
                            std::chrono::steady_clock::now();
                        gather_strategy->disable();
                        ecs_history::serialization::deserialize_registry(
                            archive,
                            this->registry.handle);
                        gather_strategy->enable();
                        uint16_t commits;
                        archive(commits);
                        for (size_t i = 0; i < commits; i++) {
                            ecs_history::commit_id base_id;
                            archive(base_id);
                            if (i == 0) {
                                base_id = ecs_history::FIRST_BASE_ID;
                            }
                            ecs_history::commit_id id;
                            archive(id);
                            auto commit = ecs_history::serialization::deserialize_commit(
                                archive);
                            this->registry.history.add_commit(base_id, id, commit);
                        }
                        const std::chrono::steady_clock::time_point end =
                            std::chrono::steady_clock::now();
                        const auto diff = std::chrono::duration_cast<std::chrono::microseconds>(
                            end - begin).count();
                        spdlog::info("applied registry with {} commits from parent in {}µs",
                                     commits,
                                     diff);
                    });
    }
};

class communication_parent_t {
    zmq::socket_t socket;
    buildit::modules::api::locked_registry_t &registry;
    std::thread thread;

public:
    explicit communication_parent_t(zmq::context_t &ctx,
                                    buildit::modules::api::locked_registry_t &registry,
                                    const std::string &communication_address)
        : socket{ctx, zmq::socket_type::server}, registry(registry) {
        spdlog::info("communication-channel started (parent)");
        this->socket.bind("tcp://" + communication_address);
        this->thread = std::thread{&communication_parent_t::receive, this};
    }

private:
    [[noreturn]] void receive() {
        while (true) {
            zmq::message_t recv_message;
            if (const zmq::recv_result_t result = this->socket.recv(recv_message); !result) {
                throw std::runtime_error("failed to receive message");
            }
            get_archive(recv_message,
                        [](cereal::PortableBinaryInputArchive &archive) {
                            deserialize_header<message_types::request_registry>(archive);
                        });
            const uint32_t routing_id = recv_message.routing_id();

            auto [string, buffer] = get_buffer(
                [this](cereal::PortableBinaryOutputArchive &archive) {
                    archive_header<message_types::registry>(archive);
                    std::shared_lock lock(this->registry.mutex);
                    const std::chrono::steady_clock::time_point begin =
                        std::chrono::steady_clock::now();
                    ecs_history::serialization::serialize_registry(
                        archive,
                        this->registry.handle);

                    size_t commits = 0;
                    size_t size = 0;
                    auto it = this->registry.history.commits.rbegin();
                    while (it != this->registry.history.commits.rend()) {
                        const size_t new_size = size + it->commit->size();
                        if (new_size > MAX_CATCHUP_SIZE && commits >= MIN_CATCHUP_COMMITS)
                            break;
                        size = new_size;
                        commits++;
                        if (commits >= MAX_CATCHUP_COMMITS)
                            break;
                        ++it;
                    }
                    auto fit = this->registry.history.commits.begin();
                    if (it != this->registry.history.commits.rend()) {
                        fit = --it.base();
                    }
                    uint16_t written_commits = commits;
                    archive(written_commits);
                    for (; fit != this->registry.history.commits.end(); ++fit) {
                        archive(fit->base_id);
                        archive(fit->id);
                        ecs_history::serialization::serialize_commit(archive, *fit->commit);
                    }
                    const std::chrono::steady_clock::time_point end =
                        std::chrono::steady_clock::now();
                    const auto diff = std::chrono::duration_cast<std::chrono::microseconds>(
                        end - begin).count();
                    spdlog::info("serialized registry with {} commits for child in {}µs",
                                 written_commits,
                                 diff);
                });

            zmq::message_t message{buffer.data(),
                                   buffer.size()};

            message.set_routing_id(routing_id);
            this->socket.send(message, zmq::send_flags::dontwait);
        }
    }
};

#endif //BUILDIT_COMMUNICATION_HPP