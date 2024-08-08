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

enum InterAction { createVertex, createVertexAndLine, moveVertex, insertVertex, insertVertexAndLine, nothing };

class Scene : public EventHandler {
public:
    explicit Scene(Programs* programs, vpSize size);
    void render();
    void use();
    Cursor cursor;

	void onResize(vpSize newSize);
	void onScroll(glm::vec2 offset);
	void onKeyAction(int key, int scanCode, int action, int mods);
	void onMouseAction(int button, int mouseAction, int mods);
    void onMouseMove(glm::vec2 abs, glm::vec2 delta);
    
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
    InterAction action = nothing; // Interaction-Action ;)
};


#endif //BUILDIT_SCENE_H
