//
// Created by felix on 7/29/24.
//

#include "graphics.h"

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <map>

std::map<int, std::string> prefixes = {{1, "K"}, {2, "M"}, {3, "G"}};
std::string formatUPS(float tps) {
    int depth = 0;
    while (tps > 10000 && depth < 3) {
        tps /= 1000;
        depth++;
    }
    return (depth > 0 ? prefixes[depth] : "") + "UPS: " + std::to_string((int) tps);
}

void Graphics::Graphics::start() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Camera2D camera = { 0, 0, 0, 0, 0.0f, 1.0f };
    camera.rotation = 0.0f;

    bool drag = false;
    Vector2 mousePos;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (drag) {
                Vector2 delta = Vector2Subtract(GetMousePosition(), mousePos);
                camera.offset = Vector2Add(camera.offset, delta);
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
            node->render(0);
        }
        EndMode2D();
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 30, WHITE);
        DrawText((formatUPS(this->simulation->currentUPS)/* + " / " + std::to_string(this->simulation->targetUPS)*/).c_str(), 10, 45, 30, WHITE);
        EndDrawing();
    }

    CloseWindow();
}

Graphics::Graphics::Graphics(Sim::Simulation *simulation) {
    this->simulation = simulation;
}

void Graphics::Graphics::addNode(Node *node) {
    this->nodes.emplace_back(node);
}

void Graphics::Graphics::addNode(GateNode<> *node) {
    this->nodes.emplace_back(node);
    this->simNodes[node->simNode] = node;
}
