//
// Created by felix on 12.04.25.
//

#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP
#include <functional>
#include <typeindex>

#include "event.hpp"

namespace Events {
    template <typename BaseEvent>
    class EventDispatcher {
        std::unordered_map<std::type_index, std::vector<std::function<void (const BaseEvent &event)>>> handlers;
    
    public:
        void dispatch(const BaseEvent &event) {
            if (const auto it = handlers.find(typeid(BaseEvent)); it != handlers.end()) {
                for (const auto& handler : it->second) {
                    handler(event);
                }
            }
        }

        template<typename Event>
        void registerHandler(std::function<void(const Event&)> handler) {
            auto& handlers = handlers[typeid(Event)];
            handlers.push_back([handler](const Event& event) {
                if (const auto* ev = dynamic_cast<const Event*>(&event)) {
                    handler(*ev);
                }
            });
        }
    };
}

#endif //EVENTDISPATCHER_HPP
