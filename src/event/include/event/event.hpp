//
// Created by felix on 11.05.25.
//

#ifndef EVENT_HPP
#define EVENT_HPP
#include <cstdint>

namespace buildit::event {

typedef struct event_t {
    const uint32_t *player_id;

    explicit event_t(const uint32_t *player_id);

    virtual ~event_t() = default;
} event_t;

class non_historical final {
public:
    ~non_historical() = default;
};
} // namespace buildit::event

#endif //EVENT_HPP
