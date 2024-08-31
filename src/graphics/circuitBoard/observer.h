//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_OBSERVER_H
#define BUILDIT_OBSERVER_H

#include <list>

template<typename T>
class Observer {
public:
    virtual void update(const T& data) = 0;
    virtual ~Observer() = default;
};

template<typename T>
class Subject {
public:
    void subscribe(Observer<T> *observer);
    void unsubscribe(Observer<T> *observer);
    virtual ~Subject() = default;
protected:
    void notify(T data);
private:
    std::list<Observer<T>*> observers;
};

template<typename T>
void Subject<T>::subscribe(Observer<T> *observer) {
    this->observers.push_back(observer);
}

template<typename T>
void Subject<T>::unsubscribe(Observer<T> *observer) {
    this->observers.remove(observer);
}

template<typename T>
void Subject<T>::notify(T data) {
    for (const auto &observer: this->observers) {
        observer->update(data);
    }
}


#endif //BUILDIT_OBSERVER_H
