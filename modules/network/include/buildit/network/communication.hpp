//
// Created by felix on 1/18/26.
//

#ifndef BUILDIT_COMMUNICATION_HPP
#define BUILDIT_COMMUNICATION_HPP
#include "commit_message.hpp"
#include "protocol.hpp"
#include "modules/module_api.hpp"
#include <zmq.hpp>
#include <zmq_addon.hpp>

class communication_child_t {
    zmq::socket_t socket;
    buildit::modules::api::locked_registry_t &registry;

public:
    void request_registry() {
        constexpr auto type = static_cast<uint8_t>(message_type_t::REQUEST_REGISTRY);
        const zmq::const_buffer buffer{&type, sizeof(type)};
        socket.send(buffer);

        std::vector<zmq::message_t> recv_msgs;
        if (const auto ret = zmq::recv_multipart(socket, std::back_inserter(recv_msgs));
            !ret || recv_msgs.size() != 1) {
            throw std::runtime_error("receiving message failed");
        }
        zmq::message_t &content = recv_msgs[0];
        zmq_membuf membuf(content.data(), content.size());
        std::istream istream(&membuf);
        cereal::PortableBinaryInputArchive archive(istream);
        // Lock scope
        {
            std::lock_guard lock(this->registry.mutex);
            ecs_history::serialization::deserialize_registry(archive, this->registry.handle);
        }
    }

    explicit communication_child_t(buildit::modules::api::locked_registry_t &registry) : registry(
        registry) {

    }
};

class communication_parent_t {
    zmq::socket_t socket;
    buildit::modules::api::locked_registry_t &registry;

    [[noreturn]] void receive() {
        while (true) {
            std::vector<zmq::message_t> recv_msgs;
            if (const auto ret = zmq::recv_multipart(socket, std::back_inserter(recv_msgs)); !ret) {
                throw std::runtime_error("receiving message failed");
            }
            zmq::message_t &header = recv_msgs[0];
            zmq_membuf membuf(header.data(), header.size());
            std::istream istream(&membuf);
            cereal::PortableBinaryInputArchive archive(istream);
            uint8_t raw_type;
            archive(raw_type);
            auto type = static_cast<message_type_t>(raw_type);
            if (type == message_type_t::REQUEST_REGISTRY) {
                std::stringstream oss{};
                // Lock scope
                {
                    cereal::PortableBinaryOutputArchive out_archive(oss);
                    std::lock_guard lock(registry.mutex);
                    ecs_history::serialization::serialize_registry(
                        out_archive,
                        this->registry.handle);
                }
                auto commit_string = oss.str();
                auto commit_data = zmq::const_buffer{commit_string.data(), commit_string.size()};
                std::array parts = {
                    commit_data
                };
                zmq::send_multipart(this->socket, parts);
            }
        }
    }

public:
    explicit communication_parent_t(buildit::modules::api::locked_registry_t &registry) : registry(
        registry) {

    }
};

#endif //BUILDIT_COMMUNICATION_HPP