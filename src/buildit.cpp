#include <thread>
#include "simulation/gate.h"
#include "graphics/graphics.h"
#include "gateNode.h"

int main() {
    Sim::NotGate sNode;
    Sim::connect(Sim::Pin{&sNode, &sNode, 0}, Sim::Pin{&sNode, &sNode, 0});

    Sim::Simulation simulation;
    simulation.addNode(&sNode);
    std::thread simTask([&simulation]() {simulation.simulate();});
    std::thread measureTask([&simulation]() {simulation.measure();});

    Graphics::Graphics graphics(&simulation);
    Graphics::GateNode gNode(100, 100, &sNode);
    graphics.addNode(&gNode);
    graphics.start();


    return 0;
}
