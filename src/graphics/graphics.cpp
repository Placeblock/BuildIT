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

    Shader shader = LoadShader(nullptr, "resources/shaders/gridShader.fs");
    int offsetLoc = GetShaderLocation(shader, "offset");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int zoomLoc = GetShaderLocation(shader, "zoom");

    float resolution[2] = { (float)screenWidth, (float)screenHeight };
    SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, zoomLoc, &this->camera.zoom, SHADER_UNIFORM_FLOAT);

    int lod = 1;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = Vector2Scale(GetMouseDelta(), -1/this->camera.zoom);
            this->camera.target = Vector2Add(this->camera.target, delta);
            updateShaderOffset(shader, offsetLoc);
        }
        float wheelMovement = GetMouseWheelMove();
        if (wheelMovement != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            this->camera.target = mouseWorldPos;
            this->camera.offset = GetMousePosition();
            this->camera.zoom += wheelMovement*0.03f;
            SetShaderValue(shader, zoomLoc, &this->camera.zoom, SHADER_UNIFORM_FLOAT);
            updateShaderOffset(shader, offsetLoc);
            if (this->camera.zoom < 1) {
                lod = floor(1.0f/this->camera.zoom);
            } else {
                lod = 1;
            }
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
        BeginMode2D(this->camera);
            for (const auto &node: nodes) {
                Vector2 worldPos = getWorldPos(node->pos);
                Vector2 screenPos = GetWorldToScreen2D(worldPos, this->camera);
                if (screenPos.x > GetScreenWidth() || screenPos.y > GetScreenHeight() ||
                    screenPos.x < 0 || screenPos.y < 0) {
                    continue;
                }
                node->render(lod);
            }
        EndMode2D();
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 30, WHITE);
        DrawText((formatUPS(this->simulation->currentUPS)/* + " / " + std::to_string(this->simulation->targetUPS)*/).c_str(), 10, 45, 30, WHITE);
        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();
}

void Graphics::Graphics::updateShaderOffset(Shader &shader, int offsetLoc) const {
    float offset[2] = {-this->camera.target.x * this->camera.zoom + this->camera.offset.x,
                       -this->camera.target.y * this->camera.zoom + this->camera.offset.y };
    SetShaderValue(shader, offsetLoc, offset, SHADER_UNIFORM_VEC2);
}

Graphics::Graphics::Graphics(Sim::Simulation *simulation) {
    this->simulation = simulation;
}

void Graphics::Graphics::addNode(Node *node) {
    this->nodes.emplace_back(node);
}
