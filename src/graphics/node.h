//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_G_NODE_H
#define BUILDIT_G_NODE_H


#include <raylib.h>
#include <vector>
#include <raymath.h>
#include "simulation/node.h"

namespace Graphics {
    class Pin;

    class Node {
    public:
        Node(Vector2 pos);

        Vector2 pos;

        virtual void render(int lod) const = 0;
    };

    class Pin {
    public:
        Pin(Sim::Reference simPin, Vector2 center);
        Sim::Reference simPin;
        Vector2 center;
        void draw(Color color) const;
    };

    static Vector2 getWorldPos(Vector2 cell) {
        return Vector2Scale(cell, 32);
    }
}


#endif //BUILDIT_G_NODE_H
