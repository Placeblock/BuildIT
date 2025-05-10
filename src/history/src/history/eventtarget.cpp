//
// Created by felix on 11.05.25.
//

#include "history/eventtarget.hpp"

template<typename E>
EventTarget<E>::EventTarget(History<E> *history)
        : history(history) {
}

template<typename E>
void EventTarget<E>::receive(const Event &event) {
    if (E* e = dynamic_cast<const E*>(&event); e != nullptr) {
        this->history->receive(e, [&] {
           this->dispatcher.dispatch(e);
        });
    }
}
