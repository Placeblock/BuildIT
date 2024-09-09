//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_OBSERVER_H
#define BUILDIT_OBSERVER_H

#include <list>
#include <memory>
#include <functional>


template<typename T>
class Subject;
template<typename T, typename C>
class TypedSubject;

template<typename T>
class Observer {
    friend class Subject<T>;
private:
    std::list<Subject<T>*> subjects;
    virtual void notify(Subject<T> *subject, const T& data) = 0;
public:
    virtual ~Observer();
};

template<typename T>
Observer<T>::~Observer() {
    for (const auto &subject: this->subjects) {
        subject->unsubscribe(this);
    }
}

template<typename T>
class Subject {
public:
    void subscribe(Observer<T> *observer);
    void unsubscribe(Observer<T> *observer);
    virtual ~Subject();
protected:
    void notify(const T& data);
private:
    std::list<Observer<T>*> observers;
};

template<typename T, typename C>
class TypedSubject : public Subject<T> {
};

template<typename T>
Subject<T>::~Subject() {
    for (const auto &observer: this->observers) {
        observer->subjects.remove(this);
    }
}

template<typename T>
void Subject<T>::subscribe(Observer<T> *observer) {
    this->observers.push_back(observer);
    observer->subjects.push_back(this);
}

template<typename T>
void Subject<T>::unsubscribe(Observer<T> *observer) {
    this->observers.remove(observer);
    observer->subjects.remove(this);
}

template<typename T>
void Subject<T>::notify(const T& data) {
    for (const auto &observer: this->observers) {
        observer->notify(this, data);
    }
}

#endif //BUILDIT_OBSERVER_H
