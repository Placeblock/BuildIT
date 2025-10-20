//
// Created by felix on 29.03.25.
//

#include "simulation/node.hpp"

using namespace sim;

pin_t::pin_t(const uint16_t type, void *value) : type(type), value(value) {

}

pin_sink_t::pin_sink_t(const uint16_t type) : type(type) {
}

node_t::node_t() = default;