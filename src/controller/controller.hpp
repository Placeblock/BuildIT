//
// Created by felix on 12.04.25.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "../events/eventdispatcher.hpp"

namespace Controller {
    class Controller {
    public:
        
    private:
        EventDispatcher<Events::Event> eventDispatcher;
    };
}
#endif //CONTROLLER_HPP
