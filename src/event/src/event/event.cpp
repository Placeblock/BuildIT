//
// Created by felix on 11.05.25.
//

#include <utility>

#include "../../include/event/event.hpp"

using namespace buildit::event;

event::event(event_type type, const player *p) : type(std::move(type)), p(p) {}