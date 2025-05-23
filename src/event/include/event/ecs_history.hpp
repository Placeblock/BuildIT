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
    explicit registry_history(ecs::registry *reg) : reg(reg) {}

    template<typename Type>
        requires(ecs::is_change_storage<ecs::registry::storage_for_type<Type>>)
    void observe() {
        using storage_type = ecs::registry::storage_for_type<Type>;
        const storage_type &storage = this->reg->storage<Type>();
    }

    void receive(std::unique_ptr<event> event, const std::function<void()> &runHandlers) override {}

private:
    ecs::registry *reg;
};

} // namespace buildit::event
#endif //ECS_HISTORY_HPP
