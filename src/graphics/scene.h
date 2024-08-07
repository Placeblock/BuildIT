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

class Scene;

class Interaction {
public:
    explicit Interaction(Scene *pScene) : scene(pScene) {};

    void update(glm::vec2 mousePos, glm::vec2 cursorPos, bool shiftClick);
    void handleLeftClick();
    void handleRightClick();
    void regenerateVisData();
    void renderVis(Program* wireShader, Program* vertexShader);
private:
    Scene* scene;
    WiresRenderer visWiresRenderer;
    std::set<std::shared_ptr<Wire>> visWires; // We have to use shared_ptr because wires system needs it
    std::set<std::shared_ptr<Vertex>> visVertices;
};

class Scene {
public:
    explicit Scene(Programs* programs, vpSize size);
    void render();
    void use();
    void updateSize(vpSize newSize);
    Cursor cursor;

    void updateCursor(glm::vec2 abs, glm::vec2 delta);

private:
    GLuint framebuffer = 0; // The framebuffer which contains our texture
    GLuint texture = 0; // The texture to render the scene into

    vpSize size;
    Programs* programs;
    Interaction interaction;
    Camera camera{};

    Wires wires{};
    Nodes nodes{};

    WiresRenderer wiresRenderer;
    GridRenderer gridRenderer;
    CursorRenderer cursorRenderer;
};


#endif //BUILDIT_SCENE_H
