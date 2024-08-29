//
// Created by felix on 8/14/24.
//

#include "horizontalList.h"
#include "graphics/gui/element.h"

using namespace GUI;

uintVec2 HorizontalList::calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) {
    unsigned int x = 0;
    for (auto it = this->children.begin(); it != iter; ++it) {
        x += (*it)->getSize().x;
    }
    return {x, 0};
}
