//
// Created by felix on 19.04.25.
//

#include "simulation/graph.hpp"
#include "simulation/simulation.hpp"
#include <iostream>
#include <thread>

class not_node_t final : public sim::node_t {
public:
    sim::pin_sink_t<bool> in;
    sim::pin_t<bool> out;

    not_node_t() : in(0), out(0, false) {
    }

    void update(const std::function<void(const sim::base_pin_t *pin)> &on_updated) override {
        if (!*this->in.pin_value != this->out.value) {
            this->out.value = !*this->in.pin_value;
            on_updated(&this->out);
        }
    }

    [[nodiscard]] std::vector<sim::base_pin_t *> get_pins() override {
        return {&out};
    }

    [[nodiscard]] std::vector<sim::base_pin_sink_t *> get_pin_sinks() override {
        return {&in};
    }
};

long long updates = 0;

[[noreturn]] void measure() {
    long long last_value = updates;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const long long value = updates;
        std::cout << value - last_value << " updates/s\n";
        last_value = value;
    }
}

int main() {
    std::thread monitor_thread(measure);

    sim::simulation_t simulation;

    not_node_t and_node;
    sim::graph::connect(and_node.out, and_node, and_node.in);

    simulation.update(&and_node);
    while (!simulation.is_empty()) {
        simulation.poll_and_update();
        updates++;
    }
}