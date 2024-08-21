//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_CIRCUITBOARD_H
#define BUILDIT_CIRCUITBOARD_H


#include "graphics/circuitBoard/elements/wires/wires.h"
#include "graphics/circuitBoard/elements/nodes/node.h"
#include "graphics/programs.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "graphics/circuitBoard/renderer/gridRenderer.h"
#include "graphics/circuitBoard/renderer/cursorRenderer.h"
#include "cursor.h"
#include "graphics/types.h"
#include "graphics/circuitBoard/data/selection.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/data/frameBufferRenderable.h"
#include "graphics/gui/widgets/image.h"

enum InterAction { modWires, moveVertex, nothing };

class CircuitBoard : public FrameBufferRenderable, public GUI::Image {
public:
    explicit CircuitBoard(GUI::View *view, uintVec2 size);
    void prerender(Programs* programs) override;
    Cursor cursor;

	void updateSize(uintVec2 newSize) override;
	void onScroll(glm::vec2 relPos, glm::vec2 offset) override;
	void onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) override;
	void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;

    void addNode(std::unique_ptr<Node> node);

    Camera camera{};

private:

    History history;

    Wires wires{};
    Nodes nodes{};

    WiresRenderer wiresRenderer;
    GridRenderer gridRenderer;
    CursorRenderer cursorRenderer;

    WiresRenderer visWiresRenderer;
    std::vector<std::unique_ptr<Wire>> visWires;
    std::vector<std::unique_ptr<Vertex>> visVertices;
    void updateVisWires();
    void createOrInsertVertex(std::unique_ptr<Vertex>& vertex);
    intVec2 calculateEndCell();

    void onClick();
    void onDragStart();
    void onDrag();
    void onDragEnd();
    void onDragSubmit();
    void onMouseDown();
    void resetAction();

	glm::vec2 mousePos;
    bool dragging = false;
    bool navigating = false;
    intVec2 clickedCell;
    Vertex* clickedVertex;
    InterAction action = nothing; // Interaction-Action ;)
    bool shift = false;
    bool ctrl = false;
    bool visualize = false;
    Selection selection;
};


#endif //BUILDIT_CIRCUITBOARD_H
