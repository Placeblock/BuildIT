//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_KIT_H
#define BUILDIT_KIT_H

#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/circuitBoard/components/renderer/renderer.h"
#include "graphics/kit/nodeList/nodeList.h"
#include "graphics/gui/widgets/horizontalList.h"

class Kit : public FrameBufferRenderable, public GUI::HorizontalList, public NodeDragHandler {
public:
    Kit(Programs *programs, GUI::View* view, uintVec2 size);
private:
    const Camera camera{}; // Default camera

    CircuitBoard* circuitBoard;
    NodeList* nodeList;

    uintVec2 calculateCBSize();
    uintVec2 calculateNLSize();

    Renderers creatingRenderers;
    std::unique_ptr<Node> creatingNode = nullptr;

    void updateSize(uintVec2 newSize) override;

    float getBoardZoom() const;
    void setCreatingNode(std::unique_ptr<Node> node) override;

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) override;
    void prerender(Programs* programs) override;
    void postrender(Programs *programs) override;
};


#endif //BUILDIT_KIT_H
