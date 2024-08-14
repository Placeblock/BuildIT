//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_CONTAINER_H
#define BUILDIT_CONTAINER_H


#include "graphics/gui/widget.h"

class Container : public Element {
protected:
    std::vector<glm::vec2> childPositions;
    virtual std::vector<glm::vec2> calcChildPositions() = 0;
public:
    uint calcBufferSize() override;

    void addChild(std::unique_ptr<Element>& child) override;
    void removeChild(Element* child) override;

    void checkChildBounds(intVec2 relPos, std::function<void(std::unique_ptr<Element> &, intVec2)> callback);

    void onMouseOver(intVec2 relPos) override;
    void onMouseMove(intVec2 relPos) override;
    void onMouseOut(intVec2 lastInPos) override;
    void onMouseAction(intVec2 relPos, int button, int mouseAction) override;

    void render(glm::vec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &texture) override;
};


#endif //BUILDIT_CONTAINER_H
