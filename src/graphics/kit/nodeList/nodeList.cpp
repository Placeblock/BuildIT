//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"
#include "notNodeElement.h"
#include "graphics/shapes/shapes.h"

NodeList::NodeList(GUI::View *view, uintVec2 size, Sim::Simulation* simulation, NodeReceiver* nodeReceiver)
    : GUI::VerticalList(view, size) {

    std::vector<float> notMeshVertices = Shapes::generateRoundedRectangle(100, 100, 5);
    std::vector<unsigned int> notMeshIndices;
    Shapes::getRoundedRectangleIndices(notMeshIndices, 0);
    std::vector<unsigned char> notMeshColors = Shapes::getRepeatedColor(Color{255, 255, 0}, notMeshVertices.size()/2);
    auto* notMeshRenderer = new InstancedMeshRenderer(notMeshVertices, notMeshColors, notMeshIndices);
    std::unique_ptr<GUI::Element> notNodeElement = std::make_unique<NotNodeElement>(view, nodeReceiver, notMeshRenderer, simulation);
    this->addChild(notNodeElement);
}

