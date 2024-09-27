
#include <thread>

#define GLM_ENABLE_EXPERIMENTAL

#include "graphics/graphics.h"


int main() {
    Sim::Simulation simulation;

    std::thread simThread(&Sim::Simulation::simulate, &simulation);
    std::thread measureThread(&Sim::Simulation::measure, &simulation);

    Graphics graphics(&simulation);
    graphics.init();
}
