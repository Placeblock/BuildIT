//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_OBSERVER_H
#define BUILDIT_OBSERVER_H

#include <list>

/**
 * This file implements the observer-pattern using the Subject and Observer class.
 * @tparam T The type of data that gets passed on notify
 * @tparam S Needed because the observer wants to receive the subject, but not only the generic Subject base-class!
 */

template<typename T, typename S>
class Subject;

template<typename T, typename S>
class Observer {
public:
    virtual void update(S *subject, const T& data) = 0;
    virtual ~Observer() = default;
};

template<typename T, typename S>
class Subject {
public:
    void subscribe(Observer<T, S> *observer);
    void unsubscribe(Observer<T, S> *observer);
    virtual S *getSubject() = 0;
    virtual ~Subject() = default;
protected:
    void notify(S *subject, T data);
private:
    std::list<Observer<T, S>*> observers;
};

template<typename T, typename S>
void Subject<T, S>::subscribe(Observer<T, S> *observer) {
    this->observers.push_back(observer);
}

template<typename T, typename S>
void Subject<T, S>::unsubscribe(Observer<T, S> *observer) {
    this->observers.remove(observer);
}

template<typename T, typename S>
void Subject<T, S>::notify(S *subject, T data) {
    for (const auto &observer: this->observers) {
        observer->update(subject, data);
    }
}


#endif //BUILDIT_OBSERVER_H
