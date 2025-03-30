#include <iostream>
#include <memory>
#include <thread>

#include "graph.h"
#include "simulation.h"

int updates = 0;

[[noreturn]] void measure() {
    while (true) {
        std::cout << updates << "\n";
        updates = 0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


int main(int argc, char *argv[]) {
    std::thread measureThread(measure);
    measureThread.detach();
    auto graph = std::make_unique<Sim::graph>();
    const auto notNode = std::make_unique<Sim::NotNode>();
    Sim::graph::connect(notNode->outputPins[0].get(), notNode.get(), 0);
    Sim::simulation simulation(graph);
    simulation.update(notNode.get());
    while (!simulation.isEmpty()) {
        simulation.pollAndUpdate();
        updates++;
    }
}
