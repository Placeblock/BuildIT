//
// Created by felix on 11.05.25.
//

#ifndef EVENTTARGET_HPP
#define EVENTTARGET_HPP
#include "event.hpp"
#include "eventdispatcher.hpp"
#include "history.hpp"

namespace buildit::event {
class event_target {
public:
    virtual ~event_target() = default;

    virtual void receive(std::unique_ptr<event> event) = 0;
};

template<typename H>
    requires(std::is_base_of_v<history<typename H::event_type>, H>)
class history_event_target final : public event_target {
    using event_type = typename H::event_type;
    using history_type = H;

    history_type *history;

public:
    event_dispatcher<event_type> dispatcher;

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

class event_target_manager {
    std::unordered_map<std::string, std::unique_ptr<event_target>> targets;

    event_target_manager() = default;
    void add(const std::string &name, std::unique_ptr<event_target> target);
    void remove(const std::string &name);
};
} // namespace buildit::event

#endif //EVENTTARGET_HPP
