//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <cstdint>
#include <functional>
#include <unordered_set>

namespace sim {
struct node_t;

struct pin_t {
    const uint16_t type;
    void *value;
    std::unordered_set<node_t *> nodes = {};

    pin_t(uint16_t type, void *value);

    bool operator==(const pin_t &pin) const {
        return pin.type == this->type;
    }
};

struct pin_sink_t {
    void *pin_value = nullptr;
    uint16_t type;

    explicit pin_sink_t(uint16_t type);
};

struct node_t {
    explicit node_t();

    virtual ~node_t() = default;

    virtual void update(const std::function<void(const pin_t *pin)> &on_updated) = 0;
};
}; // namespace sim

#endif //NODE_H
