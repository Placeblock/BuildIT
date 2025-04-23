//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <functional>
#include <unordered_set>


namespace Sim {
    class Node;

    class BasePin {
    public:
        std::unordered_set<Node *> nodes = {};
    };

    template <typename T>
    class Pin : public BasePin {
        T value = false;
    public:
        Pin();

        [[nodiscard]] T getValue() const;

        bool setValue(T value);
    };

    template <typename T>
    struct PinSink {
        Pin<T> *pin = nullptr;
    };

    class Node {
    public:
        explicit Node();

        virtual ~Node() = default;

        virtual void update(const std::function<void(const BasePin& pin)>& onUpdated) = 0;
    };
};


#endif //NODE_H
