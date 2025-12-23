//
// Created by felix on 12/23/25.
//

#ifndef BUILDIT_REGISTRY_NODE_HPP
#define BUILDIT_REGISTRY_NODE_HPP
#include "cereal/archives/portable_binary.hpp"
#include "ecs_net/component_serialization.hpp"
#include "ecs_net/registry.hpp"
#include "ecs_net/serialization.hpp"
#include "spdlog/spdlog.h"
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

class history_t {

    ecs_net::registry_t &reg;

public:
    struct history_commit_t {
        ecs_net::commit_id id;
        std::unique_ptr<ecs_net::commit_t> commit;
    };

    std::list<history_commit_t> commits{};

    explicit history_t(ecs_net::registry_t &reg)
        : reg(reg) {
    }

    void apply_commit(const ecs_net::commit_id base_id,
                      const ecs_net::commit_id id,
                      std::unique_ptr<ecs_net::commit_t> &commit) {
        spdlog::info("applying commit {}{}", id.part1, id.part2);
        auto rview = this->commits | std::views::reverse;
        const auto rit = std::ranges::find_if(rview,
                                              [&base_id](const history_commit_t &cmt) {
                                                  return cmt.id == base_id;
                                              });
        // a base_id of {0, 0} means that this was an initial commit
        if (rit == rview.end() && base_id.part1 != 0 && base_id.part2 != 0) {
            throw std::runtime_error("commit with id == base_id not found. cannot apply commit");
        }
        if (const auto it = rit.base(); it == this->commits.end()) {
            // The new commit's base_id is the last commit's id -> just insert
            this->reg.apply_commit(*commit);
            this->commits.emplace_back(id, std::move(commit));
            spdlog::info("commit is recent. applying");
        } else {
            const auto base_it = std::prev(it); // The commit with the provided base_id
            // Rollback commits after commit with base_id = id
            spdlog::info("rolling back {} commits", std::distance(it, this->commits.end()));
            for (auto rollback_it = --this->commits.end(); rollback_it != base_it; --rollback_it) {
                spdlog::info("rolling back {}{}", rollback_it->id.part1, rollback_it->id.part2);
                this->reg.apply_commit(rollback_it->commit->invert());
            }
            // Insert the new commit
            if (!this->reg.can_apply(*commit)) {
                throw std::runtime_error("Failed to apply commit received from parent");
            }
            spdlog::info("applying commit");
            this->reg.apply_commit(*commit);
            auto inserted_it = this->commits.insert(it, {id, std::move(commit)});
            // Try to reapply rolledback commits
            auto applyagain_it = ++inserted_it;
            for (; applyagain_it != this->commits.end(); ++applyagain_it) {
                spdlog::info("trying to rebase {}{}",
                             applyagain_it->id.part1,
                             applyagain_it->id.part2);
                if (this->reg.can_apply(*applyagain_it->commit)) {
                    spdlog::info("rebased {}{}",
                                 applyagain_it->id.part1,
                                 applyagain_it->id.part2);
                    this->reg.apply_commit(*applyagain_it->commit);
                } else {
                    break;
                }
            }
            spdlog::info("rebased {} commits", std::distance(inserted_it, applyagain_it));
            spdlog::info("removing {} commits (could not be rebased)",
                         std::distance(applyagain_it, this->commits.end()));
            // Remove commits we could not apply
            this->commits.erase(applyagain_it, this->commits.end());
        }
    }

    ecs_net::commit_id push_commit(const ecs_net::commit_id id,
                                   std::unique_ptr<ecs_net::commit_t> &commit) {
        spdlog::info("pushing commit %lu%lu", id.part1, id.part2);
        if (this->commits.empty()) {
            this->commits.push_back({id, std::move(commit)});
            this->reg.apply_commit(*commit);
            return {0, 0};
        }
        const auto new_base_id = commits.back().id;
        this->commits.push_back({id, std::move(commit)});
        this->reg.apply_commit(*commit);
        return new_base_id;
    }

    ecs_net::commit_id add_commit(const ecs_net::commit_id id,
                                  std::unique_ptr<ecs_net::commit_t> &commit) {
        if (this->commits.empty()) {
            this->commits.push_back({id, std::move(commit)});
            return {0, 0};
        }
        const auto new_base_id = commits.back().id;
        this->commits.push_back({id, std::move(commit)});
        return new_base_id;
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