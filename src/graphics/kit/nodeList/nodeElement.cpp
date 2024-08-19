//
// Created by felix on 8/19/24.
//

#include "nodeElement.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/font.h"
#include "graphics/gui/widgets/image.h"
#include "graphics/util.h"

NodeElement::NodeElement(GUI::View *view, const std::string& textureFile, const std::string& name, GUI::Element *parent)
    : GUI::VerticalList(view, uintVec2(160, 200), parent) {
    std::unique_ptr<GUI::Element> title = std::make_unique<GUI::Text>(view, uintVec2(), &GLOBALS::metrics, &GLOBALS::font,
                                                                      name, Alignment::CENTER, Color{255, 255, 0},
                                                                      20, this);
    this->addChild(title);
    uint texture = Util::loadTexture(textureFile);
    std::unique_ptr<GUI::Element> image = std::make_unique<GUI::Image>(view, uintVec2(160, 160), texture, this);
    this->addChild(image);
}