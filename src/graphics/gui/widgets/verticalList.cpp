//
// Created by felix on 8/14/24.
//

#include "verticalList.h"

using namespace GUI;

uintVec2 VerticalList::calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) {
    uint y = 0;
    for (auto it = this->children.begin(); it != iter; ++it) {
        y += (*it)->getSize().y;
    }
    return {0, y};
}
