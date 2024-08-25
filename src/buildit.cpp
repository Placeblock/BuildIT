
#include <thread>
#include "graphics/graphics.h"

int main() {
    Sim::Simulation simulation;

    std::thread simThread(&Sim::Simulation::simulate, &simulation);

    Graphics graphics(&simulation);
    graphics.init();
}