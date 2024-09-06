//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_OBSERVER_H
#define BUILDIT_OBSERVER_H

#include <list>
#include <functional>


template<typename T>
class Subject;

template<typename T>
class Observer {
    friend class Subject<T>;
private:
    virtual void update(const T& data) = 0;
public:
    virtual ~Observer() = default;
};

template<typename T, typename S>
class CallbackObserver : public Observer<T> {
private:
    S subject;
    std::function<void(const T& data, const S& subject)> callback;
    void update(const T& data) override;
};

template<typename T, typename S>
void CallbackObserver<T, S>::update(const T &data) {
    this->callback(data, this->subject);
}

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
