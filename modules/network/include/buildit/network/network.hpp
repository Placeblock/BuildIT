//
// Created by felix on 11.01.26.
//

#ifndef BUILDIT_NETWORK_HPP
#define BUILDIT_NETWORK_HPP
#include "commit_message.hpp"
#include "ecs_history/gather_strategy/gather_strategy.hpp"
#include <mutex>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <spdlog/spdlog.h>


class registry_node_t {
public:
    zmq::context_t context{};
    zmq::socket_t broadcast_socket{this->context, zmq::socket_type::pub};
    zmq::socket_t push_socket{this->context, zmq::socket_type::push};
    std::mutex broadcast_mutex{};
    std::mutex push_server_mutex{};

    entt::registry &reg;
    history_t history;
    std::mutex registry_mutex{};

    std::thread receive_children_thread;
    std::thread receive_parent_thread;

    bool broadcast_commits = false;
    bool push_commits = false;

    explicit registry_node_t(entt::registry &reg,
                             ecs_history::gather_strategy &gather_strategy,
                             const std::string &broadcast_address,
                             const std::string &push_address,
                             const std::string &receive_parent_address,
                             const std::string &receive_children_address)
        : reg(reg), history(reg, gather_strategy) {
        if (!broadcast_address.empty()) {
            spdlog::info("starting broadcast socket");
            this->broadcast_socket.bind("tcp://" + broadcast_address);
            this->broadcast_commits = true;
        }
        if (!push_address.empty()) {
            spdlog::info("starting push socket");
            this->push_socket.connect("tcp://" + push_address);
            this->push_commits = true;
        }
        if (!receive_children_address.empty()) {
            this->receive_children_thread = std::thread{&registry_node_t::receive_commits_children,
                                                        this, receive_children_address};
            this->receive_children_thread.detach();
        }
        if (!receive_parent_address.empty()) {
            this->receive_parent_thread = std::thread{&registry_node_t::receive_commits_parent,
                                                      this, receive_parent_address};
            this->receive_parent_thread.detach();
        }
    }

    [[noreturn]] void receive_commits_children(const std::string &receive_children_address) {
        spdlog::info("listening for commits from children");
        zmq::socket_t socket{this->context, zmq::socket_type::pull};
        socket.bind("tcp://" + receive_children_address);
        while (true) {
            std::vector<zmq::message_t> recv_msgs;
            if (const auto ret = zmq::recv_multipart(socket, std::back_inserter(recv_msgs));
                !ret || recv_msgs.size() != 2) {
                throw std::runtime_error("receiving commit failed");
            }
            commit_message_t commit_message = {
                zmq::const_buffer{recv_msgs[0].data(), recv_msgs[0].size()},
                zmq::const_buffer{recv_msgs[1].data(), recv_msgs[1].size()}};
            spdlog::info("received commit {}{} with header size {} and commit size {}",
                         commit_message.id.part1,
                         commit_message.id.part2,
                         commit_message.header.size(),
                         commit_message.commit_data.size());
            ecs_history::commit_id new_base_id{0, 0};

            // Lifetime scope (I just need this comment so that clang-format does not do wierd shit)
            {
                std::unique_lock lock(this->registry_mutex);
                if (history.is_known_commit(commit_message.id)) {
                    spdlog::info("received known commit. dropping.");
                    continue;
                }
                if (!ecs_history::can_apply_commit(this->reg, *commit_message.commit)) {
                    spdlog::info("commit cannot be applied");
                    continue;
                }
                new_base_id = history.push_commit(commit_message.id, commit_message.commit);
            }
            commit_message.base_id = new_base_id;
            if (this->broadcast_commits) {
                spdlog::info("broadcasting commit to children");
                send_commit_message(this->broadcast_socket, this->broadcast_mutex, commit_message);
            }
            if (this->push_commits) {
                spdlog::info("sending commit to parent");
                send_commit_message(this->push_socket, this->push_server_mutex, commit_message);
            }
        }
    }

    [[noreturn]] void receive_commits_parent(const std::string &receive_parent_address) {
        spdlog::info("listening for commits from parent");
        zmq::socket_t socket{this->context, zmq::socket_type::sub};
        socket.connect("tcp://" + receive_parent_address);
        socket.set(zmq::sockopt::subscribe, "");

        while (true) {
            std::vector<zmq::message_t> recv_msgs;
            if (const auto ret = zmq::recv_multipart(socket, std::back_inserter(recv_msgs));
                !ret || recv_msgs.size() != 2) {
                throw std::runtime_error("receiving commit failed");
            }
            commit_message_t commit_message = {
                zmq::const_buffer{recv_msgs[0].data(), recv_msgs[0].size()},
                zmq::const_buffer{recv_msgs[1].data(), recv_msgs[1].size()}};
            spdlog::info("received commit {}{} with header size {} and commit size {}",
                         commit_message.id.part1,
                         commit_message.id.part2,
                         commit_message.header.size(),
                         commit_message.commit_data.size());
            // Lifetime scope (I just need this comment so that clang-format does not do wierd shit)
            {
                std::unique_lock lock(this->registry_mutex);
                if (history.is_known_commit(commit_message.id)) {
                    spdlog::info("received known commit. dropping.");
                    continue;
                }
                this->history.apply_commit(commit_message.base_id,
                                           commit_message.id,
                                           commit_message.commit);
            }
            if (this->broadcast_commits) {
                spdlog::info("broadcasting commit to children");
                send_commit_message(this->broadcast_socket, this->broadcast_mutex, commit_message);
            }
        }

    }

    static void send_commit_message(zmq::socket_t &socket,
                                    std::mutex &mlock,
                                    commit_message_t &message) {
        std::ostringstream oss(std::ios::binary);
        cereal::PortableBinaryOutputArchive archive(oss);
        archive(message.base_id);
        archive(message.id);
        const auto str = oss.str();
        spdlog::info("sent commit {}{} with header size {} and commit size {}",
                     message.id.part1,
                     message.id.part2,
                     str.size(),
                     message.commit_data.size());
        // Lifetime scope
        {
            std::unique_lock lock(mlock);
            std::array parts = {
                zmq::const_buffer{str.data(), str.size()},
                message.commit_data
            };
            zmq::send_multipart(socket, parts);
        }
    }
};

#endif //BUILDIT_NETWORK_HPP