//
// Created by felix on 11.05.25.
//

#ifndef EVENTTARGET_HPP
#define EVENTTARGET_HPP
#include "entt/entt.hpp"
#include "event.hpp"
#include "history.hpp"

namespace buildit::event {
class event_target {
public:
    virtual ~event_target() = default;

    virtual void receive(std::unique_ptr<event> event) = 0;
};

template<is_history History>
class history_event_target final : public event_target {
    using event_type = typename History::event_type;
    using history_type = History;

    history_type *history;

public:
    explicit history_event_target(history_type *history) : history(history) {}

    void receive(std::unique_ptr<event> event) override {
        if (dynamic_cast<const event_type *>(event.get())) {
            std::unique_ptr<event_type> e = std::move(event);
            if (dynamic_cast<non_historical *>(event.get())) {
                this->dispatcher.dispatch(e);
            } else {
                this->history->receive(e, [&] { this->dispatcher.dispatch(e); });
            }
        }
    }
};
} // namespace buildit::event

#endif //EVENTTARGET_HPP
