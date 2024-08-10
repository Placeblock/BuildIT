
#include <iostream>
#include "graphics/graphics.h"
#include "graphics/font/FontDataLoader.h"

int main() {
    //Sim::Simulation simulation;

    //Graphics graphics(&simulation);
    //graphics.init();

    FontDataLoader fontDataLoader{"resources/font/data.fnt"};
    fontDataLoader.load();
}