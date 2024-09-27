
#include <thread>

#define GLM_ENABLE_EXPERIMENTAL

#include "graphics/graphics.h"


int main() {
    Graphics graphics;
    graphics.init();

    std::mutex lock;
    std::unique_lock<std::mutex> ulock{lock};
    std::condition_variable blockCondition;
    blockCondition.wait(ulock, []{return false;});
}
