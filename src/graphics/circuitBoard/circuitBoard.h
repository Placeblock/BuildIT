//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_CIRCUITBOARD_H
#define BUILDIT_CIRCUITBOARD_H


#include "graphics/types.h"
#include "graphics/data/frameBufferRenderable.h"
#include "graphics/gui/widgets/image.h"
#include "graphics/circuitBoard/features/feature.h"
#include "graphics/circuitBoard/components/components.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/components/renderer/componentRenderers.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/renderer/gridRenderer.h"

class CursorFeature;

class CircuitBoard : public FrameBufferRenderable, public GUI::Image, public MousePosAccessor {
public:
    explicit CircuitBoard(Programs *programs, GUI::View *view, uintVec2 size, Sim::Simulation* simulation);
    void prerender(Programs* programs) override;

	void updateSize(uintVec2 newSize) override;

    glm::vec2 getMousePos() override;

    Components components;
    History history;
    CursorFeature *cursorFeature;
    Camera camera{};
private:
    GridRenderer gridRenderer;

    std::vector<Feature*> features;
    std::vector<Updatable*> updatableFeatures;
    std::vector<Renderable*> renderableFeatures;

    CollisionDetection<Component> collisionDetection; //TODO: ADD COMPONENTS TO COLLISION DETECTION
    ComponentRenderers componentRenderers;

    Sim::Simulation* simulation;
    FontRenderer fontRenderer;
};


#endif //BUILDIT_CIRCUITBOARD_H
