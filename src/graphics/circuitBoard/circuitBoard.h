//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_CIRCUITBOARD_H
#define BUILDIT_CIRCUITBOARD_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
template class std::vector<Feature*>;

class CircuitBoard : public FrameBufferRenderable, public GUI::Image, public MousePosAccessor,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent> {
public:
    explicit CircuitBoard(Programs *programs, GUI::View *view, uintVec2 size, Sim::Simulation* simulation);
    void prerender(Programs* programs) override;

	void updateSize(uintVec2 newSize) override;

    glm::vec2 getMousePos() override;

    Components components;
    History history;
    CursorFeature *cursorFeature;
    Camera camera{};
    double lastTime = glfwGetTime();

    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onScroll(glm::vec2 relPos, glm::vec2 offset) override;
    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
    void onChar(glm::vec2 relPos, unsigned char c) override;

    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;
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
