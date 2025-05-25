//
// Created by felix on 23.04.25.
//

#ifndef NOTGATE_HPP
#define NOTGATE_HPP
#include <memory>

#include "simulation/node.hpp"


namespace NotGate {
    class Node final : public sim::node {
        sim::pin_sink<bool> inputPin;
        std::unique_ptr<sim::pin<bool>> outputPin;
    public:
        Node();
        ~Node() override = default;

        void update(const std::function<void(const sim::base_pin& pin)>& onUpdated) override;
    };
}



#endif //NOTGATE_HPP
