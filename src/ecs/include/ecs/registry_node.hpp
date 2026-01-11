//
// Created by felix on 12/23/25.
//

#ifndef BUILDIT_REGISTRY_NODE_HPP
#define BUILDIT_REGISTRY_NODE_HPP
#include "cereal/archives/portable_binary.hpp"
#include "ecs_net/component_serialization.hpp"
#include "ecs_net/registry.hpp"
#include "ecs_history/serialization.hpp"
#include <spdlog/spdlog.h>
#include <condition_variable>
#include <queue>
#include <zmq.hpp>

class zmq_membuf final : public std::streambuf {
public:
    zmq_membuf(const void *data, const std::size_t size) {
        const auto p = static_cast<char *>(const_cast<void *>(data));
        setg(p, p, p + size);
    }
};

class registry_node_t {
    ecs_net::registry_t &reg;
    zmq::context_t context{};

    std::mutex registry_mutex{};

public:
    explicit registry_node_t(ecs_net::registry_t &reg)
        : reg(reg) {
    }

    [[noreturn]] void receive_commits_children() {
        zmq::socket_t socket{this->context, ZMQ_PULL};
        socket.bind("udp://localhost:5555");

        while (true) {
            zmq::message_t message;
            const zmq::recv_result_t size = socket.recv(message);
            assert(size && "recv failed");
            zmq_membuf membuf(message.data(), message.size());
            std::istream is(&membuf);
            cereal::PortableBinaryInputArchive archive(is);
            ecs_net::commit_id id{0, 0};
            archive(id);
            auto commit = ecs_net::serialization::deserialize_commit(archive);

            // Lifetime scope (I just need this comment so that clang-format does not do wierd shit)
            {
                std::unique_lock lock(this->registry_mutex);
                if (!reg.can_apply(commit)) {
                    spdlog::info("Received commit that cannot be applied");
                    continue;
                }
                reg.apply_commit(commit);
            }

        }
    }
};

#endif //BUILDIT_REGISTRY_NODE_HPP