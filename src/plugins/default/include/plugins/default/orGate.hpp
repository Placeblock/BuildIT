//
// Created by felix on 23.04.25.
//

#ifndef ORGATE_HPP
#define ORGATE_HPP

#include <memory>

#include "simulation/node.hpp"

namespace OrGate {
    class Node final : public Sim::Node {
        std::vector<Sim::PinSink<bool>> inputPins;
        std::unique_ptr<Sim::Pin<bool>> outputPin;
    public:
        explicit Node(char inputs);

        ~Node() override = default;

        void update(const std::function<void(const Sim::BasePin& pin)>& onUpdated) override;
    };
}



#endif //ORGATE_HPP
