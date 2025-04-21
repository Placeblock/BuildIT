//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>


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

    class AndNode final : public Node {
        std::vector<PinSink<bool>> inputPins;
        std::unique_ptr<Pin<bool>> outputPin = std::make_unique<Pin<bool>>();
    public:
        explicit AndNode(char inputs);

        ~AndNode() override = default;

        void update(const std::function<void(const BasePin& pin)>& onUpdated) override;
    };

    class NotNode final : public Node {
        PinSink<bool> inputPin;
        std::unique_ptr<Pin<bool>> outputPin = std::make_unique<Pin<bool>>();
    public:
        NotNode();

        ~NotNode() override = default;

        void update(const std::function<void(const BasePin& pin)>& onUpdated) override;
    };

    class OrNode final : public Node {
        std::vector<PinSink<bool>> inputPins;
        std::unique_ptr<Pin<bool>> outputPin;
    public:
        explicit OrNode(char inputs);

        ~OrNode() override = default;

        void update(const std::function<void(const BasePin& pin)>& onUpdated) override;
    };
};


#endif //NODE_H
