//
// Created by felix on 7/29/24.
//

#include "graphics.h"

#include <raylib.h>

void Graphics::start() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");

    CloseWindow();
}
