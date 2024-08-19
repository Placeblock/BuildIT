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
    this->children.back()->updatePos(this->calcChildPosition(--this->children.end()));
}

void Container::removeChild(Element *child) {
    Element::removeChild(child);
}

void Container::onMouseOver(uintVec2 relPos) {
    this->checkChildBounds(relPos, [&relPos](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOver(uintVec2(intVec2(relPos) - pos));
    });
}

void Container::onMouseMove(uintVec2 relPos) {
    this->checkChildBounds(relPos, [&relPos](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseMove(uintVec2(intVec2(relPos) - pos));
    });
}

void Container::onMouseOut(uintVec2 lastInPos) {
    this->checkChildBounds(lastInPos, [&lastInPos](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOut(uintVec2(intVec2(lastInPos) - pos));
    });
}

void Container::onMouseAction(uintVec2 relPos, int button, int mouseAction) {
    this->checkChildBounds(relPos, [&button, &mouseAction, &relPos](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseAction(uintVec2(intVec2(relPos) - pos), button, mouseAction);
    });
}

void Container::onScroll(uintVec2 relPos, glm::vec2 offset) {
    this->checkChildBounds(relPos, [&offset, &relPos](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onScroll(uintVec2(intVec2(relPos) - pos), offset);
    });
}

void Container::checkChildBounds(uintVec2 relPos, const std::function<void(std::unique_ptr<Element> &, intVec2)>& callback) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        uintVec2 pos = (*iter)->getPos();
        uintVec2 size = (*iter)->getSize();
        if (relPos.x >= pos.x && relPos.y >= pos.y && relPos.x <= pos.x+size.x && relPos.y <= pos.y+size.y) {
            callback(*iter, pos-relPos);
        }
    }
}

void Container::render(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                       std::vector<uint> &texture) {
    for (const auto &item: this->children) {
        item->render(vertices, texCoords, colors, texture);
    }
}

void Container::updatePos(uintVec2 newPos) {
    if (newPos == this->getPos()) return;
    Element::updatePos(newPos);
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        (*it)->updatePos(this->calcChildPosition(it));
    }
}

void Container::onChildUpdateSize(Element *child) {
    auto iter = std::find_if(this->children.begin(), this->children.end(), [&](const std::unique_ptr<Element>& w) {
        return child == w.get();
    });
    while (++iter != this->children.end()) {
        (*iter)->updatePos(this->calcChildPosition(iter));
    }
}
