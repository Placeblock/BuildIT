//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_OBSERVER_H
#define BUILDIT_OBSERVER_H

#include <list>
#include <memory>
#include <functional>
#include <queue>


template<typename T>
class Subject;

template<typename T>
class Observer {
    friend class Subject<T>;
private:
    std::list<Subject<T>*> subjects;
    virtual void notify(const T& data) = 0;
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
    /**
     * We don't unsubscribe observers immediately, but add them to this queue.
     * We do this because observers could unsubscribe themselves or others while notifying them.
     */
    std::queue<Observer<T>*> unsubscribeQueue;

    void processUnsubscribeQueue();
};

template<typename T>
void Subject<T>::processUnsubscribeQueue() {
    while (!this->unsubscribeQueue.empty()) {
        Observer<T>* observer = this->unsubscribeQueue.front();
        this->observers.remove(observer);
        observer->subjects.remove(this);
        this->unsubscribeQueue.pop();
    }
}

template<typename T>
Subject<T>::~Subject() {
    for (const auto &observer: this->observers) {
        observer->subjects.remove(this);
    }
}

template<typename T>
void Subject<T>::subscribe(Observer<T> *observer) {
    this->processUnsubscribeQueue();
    this->observers.push_back(observer);
    observer->subjects.push_back(this);
}

template<typename T>
void Subject<T>::unsubscribe(Observer<T> *observer) {
    this->unsubscribeQueue.push(observer);
}

template<typename T>
void Subject<T>::notify(const T& data) {
    this->processUnsubscribeQueue();
    for (const auto &observer : this->observers) {
        observer->notify(data);
    }
}

#endif //BUILDIT_OBSERVER_H
