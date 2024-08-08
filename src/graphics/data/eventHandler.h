//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_EVENTHANDLER_H
#define BUILDIT_EVENTHANDLER_H


#include "glm/vec2.hpp"
#include "graphics/types.h"

class EventHandler {
public:
    virtual void onResize(vpSize newSize) = 0;
    virtual void onScroll(glm::vec2 offset) = 0;
    virtual void onKeyAction(int key, int scanCode, int action, int mods) = 0;
    virtual void onMouseAction(int button, int action, int mods) = 0;
    virtual void onMouseMove(glm::vec2 abs, glm::vec2 delta) = 0;
};


#endif //BUILDIT_EVENTHANDLER_H
