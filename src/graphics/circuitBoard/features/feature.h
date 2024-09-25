//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_FEATURE_H
#define BUILDIT_FEATURE_H


#include "graphics/eventHandler.h"
#include "graphics/programs.h"


class Updatable {
public:
    virtual void update(float timeStep) = 0;

    virtual ~Updatable() = default;
};

class Renderable {
protected:
    Programs *programs;
public:
    explicit Renderable(Programs *programs);
    virtual void render() = 0;

    virtual ~Renderable() = default;
};

class MousePosAccessor {
public:
    virtual glm::vec2 getMousePos() = 0;

    virtual ~MousePosAccessor() = default;
};

class Feature : public EventHandler {
};


#endif //BUILDIT_FEATURE_H
