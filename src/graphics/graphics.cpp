//
// Created by felix on 7/29/24.
//

#include "graphics.h"

#include <raylib.h>
#include <iostream>

void Graphics::start() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
        std::cout << GetFPS() << "\n";
    }

    CloseWindow();
}
