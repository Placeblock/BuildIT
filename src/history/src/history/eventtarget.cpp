//
// Created by felix on 11.05.25.
//

#include "history/eventtarget.hpp"

template<typename E>
EventTarget<E>::EventTarget(History<E> *history)
        : history(history) {
}

template<typename E>
void EventTarget<E>::receive(std::unique_ptr<Event> event) {
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
