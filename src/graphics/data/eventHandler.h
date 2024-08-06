//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_EVENTHANDLER_H
#define BUILDIT_EVENTHANDLER_H


#include "glm/vec2.hpp"

class EventHandler {
public:
    virtual void onResize(int width, int height) = 0;
    virtual void onScroll(double xOffset, double yOffset) = 0;
    virtual void onKeyAction(int key, int scanCode, int action, int mods) = 0;
    virtual void onMouseAction(int button, int action, int mods) = 0;
    virtual void onMouseMove(glm::vec2 abs, glm::vec2 delta) = 0;
};


#endif //BUILDIT_EVENTHANDLER_H
