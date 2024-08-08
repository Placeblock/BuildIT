//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_SCENE_H
#define BUILDIT_SCENE_H


#include "graphics/elements/wires/wires.h"
#include "graphics/elements/nodes/node.h"
#include "programs.h"
#include "graphics/renderer/wiresRenderer.h"
#include "graphics/renderer/gridRenderer.h"
#include "graphics/renderer/cursorRenderer.h"
#include "cursor.h"
#include "types.h"
#include "data/eventHandler.h"

enum InterAction { modWires, moveVertex, nothing };

class Scene : public EventHandler {
public:
    explicit Scene(Programs* programs, intVec2 size);
    void render();
    void use();
    Cursor cursor;

	void onResize(intVec2 newSize) override;
	void onScroll(glm::vec2 offset) override;
	void onKeyAction(int key, int scanCode, int action, int mods) override;
	void onMouseAction(int button, int mouseAction, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
    
    GLuint texture = 0; // The texture to render the scene into

private:
    GLuint framebuffer = 0; // The framebuffer which contains our texture

    intVec2 size;
    Programs* programs;
    Camera camera{};

    Wires wires{};
    Nodes nodes{};

    WiresRenderer wiresRenderer;
    GridRenderer gridRenderer;
    CursorRenderer cursorRenderer;

    WiresRenderer visWiresRenderer;
    std::shared_ptr<Wire> visWire;
    std::shared_ptr<Vertex> visStartVertex;
    std::shared_ptr<Vertex> visEndVertex;
    void updateVisWires();
    void createOrInsertVertex(const std::shared_ptr<Vertex>& vertex);
    intVec2 calculateEndCell();

	glm::vec2 mousePos;
    bool dragging = false;
    intVec2 actionCell;
    InterAction action = nothing; // Interaction-Action ;)
    bool shift = false;
    bool visualize = false;
};


#endif //BUILDIT_SCENE_H
