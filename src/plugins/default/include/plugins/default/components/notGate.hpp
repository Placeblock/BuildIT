//
// Created by felix on 23.04.25.
//

#ifndef NOTGATE_HPP
#define NOTGATE_HPP
#include <memory>

#include "simulation/node.hpp"


namespace NotGate {
    class Node final : public Sim::Node {
        Sim::PinSink<bool> inputPin;
        std::unique_ptr<Sim::Pin<bool>> outputPin;
    public:
        Node();
        ~Node() override = default;

        void update(const std::function<void(const Sim::BasePin& pin)>& onUpdated) override;
    };
}



#endif //NOTGATE_HPP
