//
// Created by felix on 11.05.25.
//

#ifndef EVENT_HPP
#define EVENT_HPP
#include <string>

namespace BuildIT {
typedef uint64_t Player;
}

//TODO: Move out of here
namespace Event {

typedef std::string EventType;

struct Event {
    const EventType type;
    const BuildIT::Player *player;

    Event(EventType type, const BuildIT::Player *player);
    virtual ~Event() = default;
};

class NonHistoryEvent {
public:
    virtual ~NonHistoryEvent() = default;
};
} // namespace Event

#endif //EVENT_HPP
