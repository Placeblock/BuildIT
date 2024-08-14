//
// Created by felix on 8/14/24.
//

#include <functional>
#include "container.h"

uint Container::calcBufferSize() {
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

void Container::onMouseOver(intVec2 relPos) {
    this->checkChildBounds(relPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOver(pos);
    });
}

void Container::onMouseMove(intVec2 relPos) {
    this->checkChildBounds(relPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseMove(pos);
    });
}

void Container::onMouseOut(intVec2 lastInPos) {
    this->checkChildBounds(lastInPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOut(pos);
    });
}

void Container::onMouseAction(intVec2 relPos, int button, int mouseAction) {
    this->checkChildBounds(relPos, [&button, &mouseAction](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseAction(pos, button, mouseAction);
    });
}

void Container::checkChildBounds(intVec2 relPos, std::function<void(std::unique_ptr<Element> &, intVec2)> callback) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        intVec2 pos = this->childPositions[i];
        if (relPos.x >= pos.x && relPos.y >= pos.y && relPos.x <= pos.x+(*iter)->size.x && relPos.y <= pos.y+(*iter)->size.y) {
            callback(*iter, pos-relPos);
        }
    }
}

void Container::render(glm::vec2 pos, std::list<float> &vertices, std::list<float> &texCoords, std::list<Color> &colors,
                       std::list<uint> &texture) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        (*iter)->render(pos+this->childPositions[i], vertices, texCoords, colors, texture);
    }
}
