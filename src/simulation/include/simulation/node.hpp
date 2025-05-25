//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <functional>
#include <unordered_set>


namespace sim {
    class node;

    class base_pin {
    public:
        std::unordered_set<node *> nodes = {};
    };

    template <typename T>
    class pin : public base_pin {
        T value = false;
    public:
        pin();

        [[nodiscard]] T get_value() const;

        bool set_value(T value);
    };

    template <typename T>
    struct pin_sink {
        pin<T> *pin = nullptr;
    };

    class node {
    public:
        explicit node();

        virtual ~node() = default;

        virtual void update(const std::function<void(const base_pin& pin)>& onUpdated) = 0;
    };
};


#endif //NODE_H
