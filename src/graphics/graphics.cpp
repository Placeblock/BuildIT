//
// Created by felix on 7/29/24.
//

#include "graphics.h"
#include "gateNode.h"

#include <raylib.h>
#include <iostream>
#include <vector>

void Graphics::Graphics::start() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;

    std::vector<GateNode> nodes(1000000, {0, 0});

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        for (const auto &node: nodes) {
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
