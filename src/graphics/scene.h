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
    void render();
private:
    Programs* programs;
    Wires* wires;
    Nodes* nodes;
};


#endif //BUILDIT_SCENE_H
