//
// Created by felix on 8/19/24.
//

#include <iostream>
#include "nodeElement.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/util.h"
#include "graphics/gui/widgets/image.h"

NodeElement::NodeElement(GUI::View *view, const std::string& textureFile, const std::string& name)
    : GUI::VerticalList(view, uintVec2(160, 176)) {
    std::unique_ptr<GUI::Element> title = std::make_unique<GUI::Text>(view, uintVec2(160, 0),
                                                                      name, Alignment::CENTER, Color{255, 255, 0},
                                                                      16);
    this->addChild(title);
    uint texture = Util::loadTexture(textureFile);
    std::unique_ptr<GUI::Element> image = std::make_unique<GUI::Image>(view, uintVec2(160, 160), texture);
    this->addChild(image);
}