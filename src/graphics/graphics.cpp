//
// Created by felix on 7/29/24.
//

#include "graphics.h"
#include "gateNode.h"

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

void Graphics::Graphics::start() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Camera2D camera = { 0, 0, 0, 0, 0.0f, 1.0f };
    camera.rotation = 0.0f;

    std::vector<GateNode> nodes(1, {0, 0});

    bool drag = false;
    Vector2 mousePos;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (drag) {
                Vector2 delta = Vector2Subtract(GetMousePosition(), mousePos);
                camera.offset = Vector2Add(camera.offset, delta);
                for (auto &node: nodes) {
                    node.updateRendered(0, camera.offset);
                }
            }
            mousePos = GetMousePosition();
            drag = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            drag = false;
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        BeginMode2D(camera);
        for (const auto &node: nodes) {
            std::cout << node.rendered << "\n";
            if (node.rendered) {
                node.render(0);
            }
        }
        EndMode2D();
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 15, BLACK);
        EndDrawing();
    }

    CloseWindow();
}
