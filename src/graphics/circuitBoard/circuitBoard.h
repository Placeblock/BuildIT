//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_CIRCUITBOARD_H
#define BUILDIT_CIRCUITBOARD_H


#include "graphics/programs.h"
#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/renderer/gridRenderer.h"
#include "graphics/circuitBoard/renderer/cursorRenderer.h"
#include "cursor.h"
#include "graphics/types.h"
#include "graphics/circuitBoard/data/selection.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/data/frameBufferRenderable.h"
#include "graphics/gui/widgets/image.h"
#include "graphics/circuitBoard/features/simulationFeature.h"
#include "graphics/font/fontRenderer.h"

class CircuitBoard : public FrameBufferRenderable, public GUI::Image {
public:
    explicit CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation);
    void prerender(Programs* programs) override;

	void updateSize(uintVec2 newSize) override;
	void onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) override;
	void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;

    void addNode(std::unique_ptr<Node> node);

    void update(Node *node);

    Camera camera{};

    Sim::Simulation* simulation;

    SimulationFeature simBridge;

    History history;
private:
    GridRenderer gridRenderer;

    FontRenderer fontRenderer;

    /**
     * Whether the ctrl key is currently pressed. Needed for actions.
     */
    bool ctrl = false;
};


#endif //BUILDIT_CIRCUITBOARD_H
