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

    [[nodiscard]] virtual std::vector<pin_t> get_pins() const = 0;

    [[nodiscard]] virtual std::vector<pin_sink_t> get_pin_sinks() const = 0;
};

template<typename T>
concept IsSimNode = std::is_base_of_v<node_t, T>;
}; // namespace sim

#endif //NODE_H
