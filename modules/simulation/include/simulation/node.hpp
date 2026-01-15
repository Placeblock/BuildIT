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

struct base_pin_t {
    const uint16_t type;
    std::unordered_set<node_t *> nodes = {};

    explicit base_pin_t(const uint16_t type_) : type(type_) {
    }

    base_pin_t(base_pin_t &&) = default;

    base_pin_t(const base_pin_t &) = default;

    virtual void *get_value_ptr() = 0;

    virtual ~base_pin_t() = default;
};

template<typename T>
struct pin_t final : base_pin_t {
    T value;

    pin_t(const uint16_t type, T value) : base_pin_t(type), value(value) {

    }

    pin_t(pin_t &&) = default;

    pin_t(const pin_t &) = default;

    bool operator==(const pin_t &pin) const {
        return pin.type == this->type;
    }

    void *get_value_ptr() override {
        return &value;
    }
};

struct base_pin_sink_t {
    uint16_t type;

    explicit base_pin_sink_t(const uint16_t type_) : type(type_) {
    }

    base_pin_sink_t(base_pin_sink_t &&) = default;

    base_pin_sink_t(const base_pin_sink_t &) = default;

    virtual void **get_pin_value() = 0;

    virtual ~base_pin_sink_t() = default;
};

template<typename T>
struct pin_sink_t final : base_pin_sink_t {
    T *pin_value = nullptr;

    explicit pin_sink_t(const uint16_t type) : base_pin_sink_t(type) {

    }

    pin_sink_t(pin_sink_t &&) = default;

    pin_sink_t(const pin_sink_t &) = default;

    void **get_pin_value() override {
        return reinterpret_cast<void **>(&this->pin_value);
    }
};

struct node_t {
    bool updated = false;

    explicit node_t();

    virtual ~node_t() = default;

    virtual void update(const std::function<void(const base_pin_t *pin)> &on_updated) = 0;

    [[nodiscard]] virtual std::vector<base_pin_t *> get_pins() = 0;

    [[nodiscard]] virtual std::vector<base_pin_sink_t *> get_pin_sinks() = 0;
};

template<typename T>
concept IsSimNode = std::is_base_of_v<node_t, T>;
}; // namespace sim

#endif //NODE_H
