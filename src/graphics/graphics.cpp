//
// Created by felix on 7/29/24.
//

#include "graphics.h"

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <map>

#define GLSL_VERSION            330

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

    Shader shader = LoadShader(nullptr, "resources/shaders/gridShader.fs");
    int offsetLoc = GetShaderLocation(shader, "offset");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int zoomLoc = GetShaderLocation(shader, "zoom");

    float resolution[2] = { (float)screenWidth, (float)screenHeight };
    SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, zoomLoc, &camera.zoom, SHADER_UNIFORM_FLOAT);

    bool drag = false;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (drag) {
                Vector2 delta = GetMouseDelta();
                camera.offset = Vector2Add(camera.offset, delta);

                float offset[2] = { camera.offset.x, camera.offset.y };
                SetShaderValue(shader, offsetLoc, offset, SHADER_UNIFORM_VEC2);
            }
            drag = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            drag = false;
        }
        float wheelMovement = GetMouseWheelMove();
        if (wheelMovement != 0) {
            camera.zoom += wheelMovement*0.01f;
            SetShaderValue(shader, zoomLoc, &camera.zoom, SHADER_UNIFORM_FLOAT);
        }
        if (IsWindowResized()) {
            resolution[0] = (float)GetScreenWidth();
            resolution[1] = (float)GetScreenHeight();
            SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
        }

        BeginDrawing();
        BeginShaderMode(shader);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
        EndShaderMode();
        BeginMode2D(camera);
            for (const auto &node: nodes) {
                node->render(0);
            }
            for (int x = 0; x < 10; ++x) {
                for (int y = 0; y < 10; ++y) {
                    DrawRectangle(x*32-2, y*32-2, 4, 4, WHITE);
                }
            }
        EndMode2D();
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 30, WHITE);
        DrawText((formatUPS(this->simulation->currentUPS)/* + " / " + std::to_string(this->simulation->targetUPS)*/).c_str(), 10, 45, 30, WHITE);
        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();
}

Graphics::Graphics::Graphics(Sim::Simulation *simulation) {
    this->simulation = simulation;
}

void Graphics::Graphics::addNode(Node *node) {
    this->nodes.emplace_back(node);
}
