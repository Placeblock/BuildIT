#include <thread>
#include "simulation/gate.h"
#include "graphics/graphics.h"
#include "gateNode.h"
#include "gates/notGateNode.h"
#include "gates/andGateNode.h"

int main() {
    Sim::NotGate sNode;

    Sim::AndGate sAndNode;
    sAndNode.setInput(1, true);
    Sim::connect(Sim::Reference{&sNode, &sAndNode, 0}, Sim::Reference{&sAndNode, &sNode, 0});
    Sim::connect(Sim::Reference{&sAndNode, &sNode, 0}, Sim::Reference{&sNode, &sAndNode, 0});

    Sim::Simulation simulation;
    simulation.addNode(&sNode);
    simulation.addNode(&sAndNode);
    std::thread simTask([&simulation]() {simulation.simulate();});
    std::thread measureTask([&simulation]() {simulation.measure();});

    Graphics::Graphics graphics(&simulation);
    Graphics::NotGateNode gNode(Vector2(100, 100), &sNode);
    Graphics::AndGateNode gAndNode(Vector2(300, 125), &sAndNode);
    graphics.addNode(&gNode);
    graphics.addNode(&gAndNode);
    graphics.start();


    return 0;
}
