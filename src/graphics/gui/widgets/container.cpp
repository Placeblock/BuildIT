//
// Created by felix on 8/14/24.
//

#include "container.h"
#include <algorithm>

using namespace GUI;

unsigned int Container::calcBufferSize() const {
    return 0;
}

void Container::addChild(std::unique_ptr<Element> &child) {
    Element::addChild(child);
    this->children.back()->updatePos(this->calcChildPosition(--this->children.end()));
}

void Container::removeChild(Element *child) {
    Element::removeChild(child);
}

void Container::onMouseOver(glm::vec2 relPos) {
    Element::onMouseOver(relPos);
    this->checkChildBounds(relPos, [](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseOver(pos);
    });
}

void Container::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    this->checkChildBounds(relPos, [&delta, this](std::unique_ptr<Element>& child, intVec2 pos) {
        for (const auto &child2: this->children) {
            if (child2 != child && child2->mouseOver) {
                child2->onMouseOut();
            }
        }
        if (!child->mouseOver) {
            child->onMouseOver(pos);
        }
        child->onMouseMove(pos, delta);
    });
}

void Container::onMouseOut() {
    if (this->mouseOver) {
        for (auto & it : this->children) {
            it->onMouseOut();
        }
    }
    Element::onMouseOut();
}

void Container::onMouseAction(glm::vec2 relPos, int button, int mouseAction) {
    this->checkChildBounds(relPos, [&button, &mouseAction](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onMouseAction(pos, button, mouseAction);
    });
}

void Container::onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) {
    this->checkChildBounds(relPos, [&key, &scanCode, &keyAction, &mods](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onKeyAction(pos, key, scanCode, keyAction, mods);
    });
}


void Container::onScroll(glm::vec2 relPos, glm::vec2 offset) {
    this->checkChildBounds(relPos, [&offset](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onScroll(pos, offset);
    });
}

void Container::onChar(glm::vec2 relPos, unsigned char c) {
    this->checkChildBounds(relPos, [&c](std::unique_ptr<Element>& child, intVec2 pos) {
        child->onChar(pos, c);
    });
}

void Container::checkChildBounds(glm::vec2 relPos, const std::function<void(std::unique_ptr<Element> &, uintVec2)>& callback) {
    int i = 0;
    for (auto iter = this->children.begin(); iter != this->children.end(); iter++, i++) {
        uintVec2 pos = (*iter)->getRelPos();
        uintVec2 size = (*iter)->getSize();
        if (relPos.x >= float(pos.x) && relPos.y >= float(pos.y) && relPos.x <= float(pos.x+size.x) && relPos.y <= float(pos.y+size.y)) {
            callback(*iter, uintVec2(relPos) - pos);
        }
    }
}

void Container::generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                       std::vector<uint> &texture) {
    for (const auto &item: this->children) {
        item->generateBuffer(vertices, texCoords, colors, texture);
    }
}

void Container::updatePos(uintVec2 newRelPos) {
    if (newRelPos == this->getRelPos()) return;
    Element::updatePos(newRelPos);
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
