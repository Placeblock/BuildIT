//
// Created by felix on 11.05.25.
//

#ifndef EVENTTARGET_HPP
#define EVENTTARGET_HPP
#include "event.hpp"
#include "history.hpp"
#include "eventdispatcher.hpp"

template <typename E>
class EventTarget {
    History<E> *history;
public:
    Events::EventDispatcher<E> dispatcher;

    explicit EventTarget(History<E> *history);

    void receive(const Event& event);
};



#endif //EVENTTARGET_HPP
