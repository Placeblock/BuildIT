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
        Node(Vector2 pos, std::vector<Pin> inputPins, std::vector<Pin> outputPins);

        Vector2 pos;
        std::vector<Pin> inputPins;
        std::vector<Pin> outputPins;

        virtual void updatePinPosition() = 0;
        virtual void render(int lod) const = 0;
    };

    class Pin {
    public:
        Pin(Sim::Reference simPin, Vector2 center);
        Sim::Reference simPin;
        Vector2 center;
        void draw(Color color) const;

        static std::vector<Pin> convert(std::vector<Sim::Reference> simPin) {

        }
    };
}


#endif //BUILDIT_G_NODE_H
