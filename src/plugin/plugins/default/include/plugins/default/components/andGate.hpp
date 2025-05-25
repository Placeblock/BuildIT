//
// Created by felix on 23.04.25.
//

#ifndef ANDGATE_HPP
#define ANDGATE_HPP
#include <memory>

#include "simulation/node.hpp"

namespace AndGate {
    class Node final : public sim::node {
        std::vector<sim::pin_sink<bool>> inputPins;
        std::unique_ptr<sim::pin<bool>> outputPin;
    public:
        explicit Node(char inputs);

        ~Node() override = default;

        void update(const std::function<void(const sim::base_pin& pin)>& onUpdated) override;
    };
}


#endif //ANDGATE_HPP
