//
// Created by felix on 23.04.25.
//

#ifndef ANDGATE_HPP
#define ANDGATE_HPP
#include <memory>

#include "simulation/node.hpp"

namespace AndGate {
    class Node final : public Sim::Node {
        std::vector<Sim::PinSink<bool>> inputPins;
        std::unique_ptr<Sim::Pin<bool>> outputPin;
    public:
        explicit Node(char inputs);

        ~Node() override = default;

        void update(const std::function<void(const Sim::BasePin& pin)>& onUpdated) override;
    };
    struct Component {
        std::unique_ptr<Node> node;
    };
}


#endif //ANDGATE_HPP
