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
public:
    CallbackObserver(S subject, std::function<void(const T& data, const S subject)> callback)
        : subject(subject), callback(callback) {};
private:
    S subject;
    std::function<void(const T& data, S subject)> callback;
    void update(const T& data) override;
};


template<typename T, typename S>
class MultiObserver {
private:
    std::unordered_map<S, std::unique_ptr<CallbackObserver<T, S>>> observers;
public:
    CallbackObserver<T, S> *addSubject(S subject);
    CallbackObserver<T, S> *removeSubject(S subject);
    virtual void update(const T& data, S subject) = 0;
    virtual ~MultiObserver();
};

template<typename T, typename S>
CallbackObserver<T, S> *MultiObserver<T, S>::removeSubject(S subject) {
    CallbackObserver<T, S> *obs = this->observers[subject].release();
    this->observers.erase(subject);
    return obs;
}

template<typename T, typename S>
CallbackObserver<T, S> *MultiObserver<T, S>::addSubject(S subject) {
    auto obs = std::make_unique<CallbackObserver<T, S>>(subject, [this](const T& data, S subject){
        this->update(data, subject);
    });
    this->observers[subject] = std::move(obs);
    return this->observers[subject].get();
}

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
    void notify(const T& data);
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
void Subject<T>::notify(const T& data) {
    for (const auto &observer: this->observers) {
        observer->update(data);
    }
}


#endif //BUILDIT_OBSERVER_H
