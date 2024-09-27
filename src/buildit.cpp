
#include <thread>

#define GLM_ENABLE_EXPERIMENTAL

#include "graphics/graphics.h"


int main() {
    Sim::Simulation simulation;

    Graphics graphics(&simulation);
    std::thread graphicThread(&Graphics::init, &graphics);

    simulation.start();

    std::mutex lock;
    std::unique_lock<std::mutex> ulock{lock};
    std::condition_variable blockCondition;
    blockCondition.wait(ulock, []{return false;});
}
