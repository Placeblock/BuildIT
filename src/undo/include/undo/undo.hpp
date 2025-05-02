//
// Created by felix on 02.05.25.
//

#ifndef UNDO_HPP
#define UNDO_HPP
#include <functional>
#include <typeindex>

typedef std::tuple<std::type_index, std::type_index> EventPair;
class Conflicts {
    std::unordered_map<EventPair, std::function<bool(const void*, const void*)>> checkers;

    template<typename A, typename B>
    void registerHandler(std::function<bool(const A*, const B*)> checker);

    template<typename A, typename B>
    bool hasConflict(const A& a, const B& b) {
        const EventPair pair = std::make_tuple(typeid(A), typeid(B));
        if (this->checkers.find(pair) == this->checkers.end()) {
            return true;
        }
        return this->checkers[pair](a, b);
    }
};

template<typename A, typename B>
void Conflicts::registerHandler(std::function<bool(const A*, const B*)> checker) {
    this->checkers.emplace(std::make_tuple(typeid(A), typeid(B)), checker);
}

template<typename A>
struct EventHandle {
    void *event;
    A author;
    EventHandle *undoneBy;
};

template<typename A>
class History {
    std::vector<EventHandle<A>> events;
};

#endif //UNDO_HPP
