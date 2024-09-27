//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_EVENTHANDLER_H
#define BUILDIT_EVENTHANDLER_H


#include "glm/vec2.hpp"

class EventHandler {
public:
    bool mouseOver = false;
    virtual void onMouseOver(glm::vec2 relPos) {this->mouseOver = true;};
    virtual void onMouseOut() {this->mouseOver = false;};
    virtual void onMouseMove(glm::vec2 relPos, glm::vec2 delta) {};
    virtual void onMouseAction(glm::vec2 relPos, int button, int action, int mods) {};
    virtual void onScroll(glm::vec2 relPos, glm::vec2 offset) {};
    virtual void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {};
    virtual void onChar(glm::vec2 relPos, unsigned char c) {};

    virtual ~EventHandler() = default;
};


#endif //BUILDIT_EVENTHANDLER_H
