//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_SCENE_H
#define BUILDIT_SCENE_H


#include "graphics/elements/wires/wires.h"
#include "graphics/elements/nodes/node.h"
#include "programs.h"

class Scene {
public:
    explicit Scene(Programs* programs) : programs(programs) {};
    void render();
private:
    Programs* programs;
    Camera camera{};
    Wires* wires{};
    Nodes* nodes{};
};


#endif //BUILDIT_SCENE_H
