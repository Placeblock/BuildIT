//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_CIRCUITBOARD_H
#define BUILDIT_CIRCUITBOARD_H


#include "graphics/circuitBoard/components/wires/wires.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
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
#include "graphics/circuitBoard/components/simulationBridge.h"
#include "graphics/font/fontRenderer.h"

enum InterAction { modWires, moveVertex, nothing };

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

    SimulationBridge simBridge;

    History history;
private:
    GridRenderer gridRenderer;

    FontRenderer fontRenderer;

    WiresRenderer visWiresRenderer;
    std::unique_ptr<Network> visNetwork = std::make_unique<Network>(glm::vec3{1.0f, 0.8f, 1.0f});
    std::vector<std::unique_ptr<Wire>> visWires;
    std::vector<std::unique_ptr<Joint>> visJoints;
    void updateVisWires();
    void createOrInsertJoint(std::unique_ptr<Joint>& joint);
    intVec2 calculateEndCell();

    void onClick();
    void onDragStart();
    void onDrag();
    void onDragEnd();
    void onDragSubmit();
    void onMouseDown();
    void resetAction();

    /**
     * Whether the user is dragging something (e.g. while moving something or creating wires
     */
    bool dragging = false;
    /**
     * Stores the last clicked pos to access later
     */
    intVec2 clickedCell;
    /**
     * Stores the last clicked vertex to access later
     * (could be accessed from clickedCell but is cached because of performance)
     */
    Joint *clickedJoint = nullptr;
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
     * @param cell The pos to check
     * @return Whether you can start creating wires (mod wires) without the shift key for example when hovering a vertex
     */
    bool canModWiresNoShift(intVec2 cell);
    /**
     * @param cell The pos to check
     * @return Whether you can start creating wires. Could return false because for example a node
     */
    bool canModWires(intVec2 cell);
};


#endif //BUILDIT_CIRCUITBOARD_H
