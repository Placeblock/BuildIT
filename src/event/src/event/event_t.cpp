//
// Created by felix on 11.05.25.
//

#include <utility>

#include "event/event.hpp"

using namespace buildit::event;

event_t::event_t(const uint32_t *player_id) : player_id(player_id) {
}