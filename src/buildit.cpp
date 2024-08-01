#include <thread>
#include <iostream>
#include "simulation/gate.h"
#include "graphics/graphics.h"
#include "gateNode.h"
#include "gates/andGateNode.h"

int main() {
    Sim::AndGate sAndNode;
    sAndNode.setInput(1, true);

    Sim::Simulation simulation;
    simulation.addNode(&sAndNode);
    std::thread simTask([&simulation]() {simulation.simulate();});
    std::thread measureTask([&simulation]() {simulation.measure();});

    Graphics::Graphics graphics(&simulation);
    for (int i = 0; i < 10; i++) {
        int x = GetRandomValue(0, 100);
        int y = GetRandomValue(0, 100);
        std::cout << x << " | " << y << "\n";
        auto* gAndNode = new Graphics::AndGateNode(Vector2(x, y), &sAndNode);
        graphics.addNode(gAndNode);
    }
    graphics.start();


    return 0;
}
