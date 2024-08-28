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
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"

enum InterAction { modWires, moveVertex, nothing };

class CircuitBoard : public FrameBufferRenderable, public GUI::Image {
public:
    explicit CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation);
    void prerender(Programs* programs) override;
    Cursor cursor;

	void updateSize(uintVec2 newSize) override;
	void onScroll(glm::vec2 relPos, glm::vec2 offset) override;
	void onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) override;
	void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;

    void addNode(std::unique_ptr<Node> node);

    void update(Node *node);

    Camera camera{};

    Sim::Simulation* simulation;

    NodeRenderers nodeRenderers{};
    Nodes nodes{};

    History history;
private:

    Wires wires{};

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

    /**
     * Whether the user is moving the board around
     */
    bool navigating = false;
    /**
     * Whether the user is dragging something (e.g. while moving something or creating wires
     */
    bool dragging = false;
    /**
     * Stores the last clicked cell to access later
     */
    intVec2 clickedCell;
    /**
     * Stores the last clicked vertex to access later
     * (could be accessed from clickedCell but is cached because of performance)
     */
    Vertex* clickedVertex = nullptr;
    /**
     * The last currently performed action
     */
    InterAction action = nothing;
    /**
     * Whether the shift key is currently pressed. Needed for actions.
     */
    bool shift = false;
    /**
     * Whether the ctrl key is currently pressed. Needed for actions.
     */
    bool ctrl = false;
    /**
     * Whether the visualization should be rendered (For moving and creating wires)
     */
    bool visualize = false;
    /**
     * The selection of wires, joints, and Nodes
     */
    Selection selection;

    /**
     * @param cell The cell to check
     * @return Whether you can start creating wires (mod wires) without the shift key for example when hovering a vertex
     */
    bool canModWiresNoShift(intVec2 cell);
    /**
     * @param cell The cell to check
     * @return Whether you can start creating wires. Could return false because for example a node
     */
    bool canModWires(intVec2 cell);
};


#endif //BUILDIT_CIRCUITBOARD_H
