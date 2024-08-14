//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_CONTAINER_H
#define BUILDIT_CONTAINER_H


#include "graphics/gui/widget.h"

class Container : public Element {
protected:
    std::vector<uintVec2> childPositions;
    virtual std::vector<uintVec2> calcChildPositions() = 0;
public:
    Container(GUI* gui, uintVec2 size) : Element(gui, size) {};
    uint calcBufferSize() override;

    void addChild(std::unique_ptr<Element>& child) override;
    void removeChild(Element* child) override;

    void checkChildBounds(uintVec2 relPos, const std::function<void(std::unique_ptr<Element> &, intVec2)>& callback);

    void onMouseOver(uintVec2 relPos) override;
    void onMouseMove(uintVec2 relPos) override;
    void onMouseOut(uintVec2 lastInPos) override;
    void onMouseAction(uintVec2 relPos, int button, int mouseAction) override;

    void render(uintVec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &texture) override;
};


#endif //BUILDIT_CONTAINER_H
