//
// Created by felix on 23.05.25.
//

#ifndef ECS_HISTORY_HPP
#define ECS_HISTORY_HPP
#include "event.hpp"
#include "history.hpp"
#include "model/changemixin.hpp"
#include "model/collision.hpp"

namespace buildit::event {
    class registry_history final : public history<event> {
    public:
        explicit registry_history(ecs::registry *reg) : reg(reg) {
        }

        void receive(std::unique_ptr<event> event, const std::function<void()> &runHandlers) override;

    private:
        ecs::registry *reg;
    };
} // namespace buildit::event
#endif //ECS_HISTORY_HPP
