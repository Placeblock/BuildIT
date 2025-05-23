//
// Created by felix on 12.04.25.
//

#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP
#include <functional>
#include <typeindex>

namespace buildit::event {
template<typename BaseEvent>
class event_dispatcher {
    using base_event = BaseEvent;

    std::unordered_map<std::type_index, std::vector<std::function<void(const base_event& event)>>>
        handlers;

public:
    void dispatch(const base_event& event) {
        if (const auto it = handlers.find(typeid(base_event)); it != handlers.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }

    template<typename Event>
        requires std::is_base_of_v<base_event, Event>
    void register_handler(std::function<void(const Event&)> handler) {
        auto& handlers = this->handlers[typeid(Event)];
        handlers.push_back([handler](const Event& event) {
            if (const auto* ev = dynamic_cast<const Event*>(&event)) {
                handler(*ev);
            }
        });
    }
};
} // namespace buildit::event

#endif //EVENTDISPATCHER_HPP
