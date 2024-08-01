//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_G_NODE_H
#define BUILDIT_G_NODE_H


#include <raylib.h>
#include <vector>
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
}


#endif //BUILDIT_G_NODE_H
