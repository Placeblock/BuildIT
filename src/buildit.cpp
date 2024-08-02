#include <thread>
#include <iostream>
#include "simulation/gate.h"
#include "graphics/graphics.h"
#include "gateNode.h"
#include "gates/notGateNode.h"

int main() {
    Sim::NotGate sAndNode;
    sAndNode.setInput(1, true);
    Sim::connect(Sim::Reference{&sAndNode, &sAndNode, 0}, Sim::Reference{&sAndNode, &sAndNode, 0});

    Sim::Simulation simulation;
    simulation.addNode(&sAndNode);
    std::thread simTask([&simulation]() {simulation.simulate();});
    std::thread measureTask([&simulation]() {simulation.measure();});

    Graphics::Graphics graphics(&simulation);
    auto* gAndNode = new Graphics::NotGateNode(Vector2(10, 10), &sAndNode);
    graphics.addNode(gAndNode);
    graphics.start();


    return 0;
}
