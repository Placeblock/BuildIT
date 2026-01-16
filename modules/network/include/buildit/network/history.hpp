//
// Created by felix on 1/16/26.
//

#ifndef BUILDIT_HISTORY_HPP
#define BUILDIT_HISTORY_HPP
#include "ecs_history/commit.hpp"
#include <spdlog/spdlog.h>

class history_t {
    entt::registry &reg;
    ecs_history::gather_strategy &gather_strategy;

public:
    struct history_commit_t {
        ecs_history::commit_id id;
        std::unique_ptr<ecs_history::commit_t> commit;
    };

    std::list<history_commit_t> commits{};

    explicit history_t(entt::registry &reg, ecs_history::gather_strategy &gather_strategy)
        : reg(reg), gather_strategy(gather_strategy) {
    }

    void apply_commit(const ecs_history::commit_id base_id,
                      const ecs_history::commit_id id,
                      std::unique_ptr<ecs_history::commit_t> &commit) {
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
            spdlog::info("commit is recent. applying");
            ecs_history::apply_commit(this->reg, this->gather_strategy, *commit);
            this->commits.emplace_back(id, std::move(commit));
        } else {
            const auto base_it = std::prev(it); // The commit with the provided base_id
            // Rollback commits after commit with base_id = id
            spdlog::info("rolling back {} commits", std::distance(it, this->commits.end()));
            for (auto rollback_it = --this->commits.end(); rollback_it != base_it; --rollback_it) {
                spdlog::info("rolling back {}{}", rollback_it->id.part1, rollback_it->id.part2);
                ecs_history::apply_commit(this->reg,
                                          this->gather_strategy,
                                          *rollback_it->commit->invert());
            }
            // Insert the new commit
            if (!ecs_history::can_apply_commit(this->reg, *commit)) {
                throw std::runtime_error("Failed to apply commit received from parent");
            }
            spdlog::info("applying commit");
            ecs_history::apply_commit(this->reg, this->gather_strategy, *commit);
            auto inserted_it = this->commits.insert(it, {id, std::move(commit)});
            // Try to reapply rolledback commits
            auto applyagain_it = ++inserted_it;
            for (; applyagain_it != this->commits.end(); ++applyagain_it) {
                spdlog::info("trying to rebase {}{}",
                             applyagain_it->id.part1,
                             applyagain_it->id.part2);
                if (ecs_history::can_apply_commit(this->reg, *applyagain_it->commit)) {
                    spdlog::info("rebased {}{}",
                                 applyagain_it->id.part1,
                                 applyagain_it->id.part2);
                    ecs_history::apply_commit(this->reg,
                                              this->gather_strategy,
                                              *applyagain_it->commit);
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

    ecs_history::commit_id push_commit(const ecs_history::commit_id id,
                                       std::unique_ptr<ecs_history::commit_t> &commit) {
        spdlog::info("pushing commit {}{}", id.part1, id.part2);
        if (this->commits.empty()) {
            this->commits.push_back({id, std::move(commit)});
            ecs_history::apply_commit(this->reg,
                                      this->gather_strategy,
                                      *this->commits.back().commit);
            return {0, 0};
        }
        const auto new_base_id = commits.back().id;
        this->commits.push_back({id, std::move(commit)});
        ecs_history::apply_commit(this->reg, this->gather_strategy, *this->commits.back().commit);
        return new_base_id;
    }

    ecs_history::commit_id add_commit(const ecs_history::commit_id id,
                                      std::unique_ptr<ecs_history::commit_t> &commit) {
        if (this->commits.empty()) {
            this->commits.push_back({id, std::move(commit)});
            return {0, 0};
        }
        const auto new_base_id = commits.back().id;
        this->commits.push_back({id, std::move(commit)});
        return new_base_id;
    }

    bool is_known_commit(const ecs_history::commit_id id) {
        return std::ranges::any_of(this->commits,
                                   [&id](const auto &commit) {
                                       return commit.id == id;
                                   });
    }
};

#endif //BUILDIT_HISTORY_HPP