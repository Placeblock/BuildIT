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

enum InterAction { drawWires, moveVertex, nothing };

class Scene : public EventHandler {
public:
    explicit Scene(Programs* programs, vpSize size);
    void render();
    void use();
    Cursor cursor;

	void onResize(vpSize newSize) override;
	void onScroll(glm::vec2 offset) override;
	void onKeyAction(int key, int scanCode, int action, int mods) override;
	void onMouseAction(int button, int mouseAction, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
    
    GLuint texture = 0; // The texture to render the scene into

private:
    GLuint framebuffer = 0; // The framebuffer which contains our texture

    vpSize size;
    Programs* programs;
    Camera camera{};

    Wires wires{};
    Nodes nodes{};

    WiresRenderer wiresRenderer;
    GridRenderer gridRenderer;
    CursorRenderer cursorRenderer;

	glm::vec2 mousePos;
    bool dragging = false;
    glm::vec2 actionStart;
    std::shared_ptr<Vertex> lastVertex;
    InterAction action = nothing; // Interaction-Action ;)
    bool shift = false;
};


#endif //BUILDIT_SCENE_H
