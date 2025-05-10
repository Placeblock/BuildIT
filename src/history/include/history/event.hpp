//
// Created by felix on 11.05.25.
//

#ifndef EVENT_HPP
#define EVENT_HPP
#include <string>


typedef uint64_t Player;

typedef std::string EventType;

struct Event {
    const EventType type;
    const Player *player;
};

struct NonHistoryEvent {};



#endif //EVENT_HPP
