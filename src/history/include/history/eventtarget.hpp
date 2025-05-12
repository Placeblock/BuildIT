//
// Created by felix on 11.05.25.
//

#ifndef EVENTTARGET_HPP
#define EVENTTARGET_HPP
#include "event.hpp"
#include "history.hpp"
#include "eventdispatcher.hpp"

class BaseEventTarget {
public:
    virtual ~BaseEventTarget() = default;

    virtual void receive(std::unique_ptr<Event> event) = 0;
};

template <typename E>
class EventTarget : public BaseEventTarget {
    History<E> *history;
public:
    Events::EventDispatcher<E> dispatcher;

    explicit EventTarget(History<E> *history)
        : history(history) {
    }

    void receive(std::unique_ptr<Event> event) override  {
        if (dynamic_cast<const E*>(event.get())) {
            std::unique_ptr<E> e = std::move(event);
            if (dynamic_cast<NonHistoryEvent*>(event.get())) {
                this->dispatcher.dispatch(e);
            } else {
                this->history->receive(e, [&] {
                   this->dispatcher.dispatch(e);
                });
            }
        }
    }
};

class EventTargetManager {
    std::unordered_map<std::string, std::unique_ptr<BaseEventTarget>> targets;

    EventTargetManager() = default;
    void add(const std::string &name, std::unique_ptr<BaseEventTarget> target);
    void remove(const std::string &name);
};



#endif //EVENTTARGET_HPP
