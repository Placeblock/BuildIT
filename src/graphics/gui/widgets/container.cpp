//
// Created by felix on 8/14/24.
//

#include <functional>
#include "container.h"

using namespace GUI;

uint Container::calcBufferSize() const {
    return 0;
}

void Container::addChild(std::unique_ptr<Element> &child) {
    Element::addChild(child);
    this->childPositions = this->calcChildPositions();
}

void Container::removeChild(Element *child) {
    Element::removeChild(child);
    this->childPositions = this->calcChildPositions();
}

void Container::onMouseOver(uintVec2 relPos) {
    this->checkChildBounds(relPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOver(pos);
    });
}

void Container::onMouseMove(uintVec2 relPos) {
    this->checkChildBounds(relPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseMove(pos);
    });
}

void Container::onMouseOut(uintVec2 lastInPos) {
    this->checkChildBounds(lastInPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOut(pos);
    });
}

void Container::onMouseAction(uintVec2 relPos, int button, int mouseAction) {
    this->checkChildBounds(relPos, [&button, &mouseAction](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseAction(pos, button, mouseAction);
    });
}

void Container::checkChildBounds(uintVec2 relPos, const std::function<void(std::unique_ptr<Element> &, intVec2)>& callback) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        uintVec2 pos = this->childPositions[i];
        if (relPos.x >= pos.x && relPos.y >= pos.y && relPos.x <= pos.x+(*iter)->size.x && relPos.y <= pos.y+(*iter)->size.y) {
            callback(*iter, pos-relPos);
        }
    }
}

void Container::render(uintVec2 pos, std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                       std::vector<uint> &texture) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        (*iter)->render(pos+this->childPositions[i], vertices, texCoords, colors, texture);
    }
}
