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
#include "graphics/circuitBoard/features/cablingFeature.h"
#include "graphics/circuitBoard/features/cursorFeature.h"
#include "graphics/circuitBoard/features/deleteFeature.h"
#include "graphics/circuitBoard/features/modifyCablingFeature.h"
#include "graphics/circuitBoard/features/moveFeature.h"
#include "graphics/circuitBoard/features/navigationFeature.h"
#include "graphics/circuitBoard/features/nodesFeature.h"

class CircuitBoard : public FrameBufferRenderable, public GUI::Image {
public:
    explicit CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation);
    void prerender(Programs* programs) override;

	void updateSize(uintVec2 newSize) override;
private:
    GridRenderer gridRenderer;

    SimulationFeature simulationFeature;
    CablingFeature cablingFeature;
    CursorFeature cursorFeature;
    DeleteFeature deleteFeature;
    ModifyCablingFeature modifyCablingFeature;
    MoveFeature moveFeature;
    NavigationFeature navigationFeature;
    NodesFeature nodesFeature;
    SelectionFeature selectionFeature;

    Camera camera{};
    Sim::Simulation* simulation;
    History history;
    FontRenderer fontRenderer;
};


#endif //BUILDIT_CIRCUITBOARD_H
