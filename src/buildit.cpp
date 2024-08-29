
#include <thread>

#define GLM_ENABLE_EXPERIMENTAL

#include "graphics/graphics.h"


int main() {
    Sim::Simulation simulation;

    std::thread simThread(&Sim::Simulation::simulate, &simulation);

    Graphics graphics(&simulation);
    graphics.init();
}
