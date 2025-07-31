//
// Created by felix on 11.05.25.
//

#ifndef EVENT_HPP
#define EVENT_HPP
#include "common/player.hpp"
#include <string>

namespace buildit::event {

typedef std::string event_type;

struct event {
    const event_type type;
    const player_id *p;

    event(event_type type, const player_id *p);
    virtual ~event() = default;
};

class non_historical final {
public:
    ~non_historical() = default;
};
} // namespace buildit::event

#endif //EVENT_HPP
