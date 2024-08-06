
#include <iostream>
#include "graphics/graphics.h"

int main() {
    Sim::Simulation simulation;

    Graphics graphics(&simulation);
    graphics.init();
}