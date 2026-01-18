//
// Created by felix on 1/18/26.
//

#ifndef BUILDIT_PROTOCOL_HPP
#define BUILDIT_PROTOCOL_HPP
#include <cstdint>

constexpr uint32_t VERSION = 0;

enum class message_type_t {
    REQUEST_REGISTRY = 0,
    REGISTRY = 1,
    COMMIT = 2,
};

#endif //BUILDIT_PROTOCOL_HPP