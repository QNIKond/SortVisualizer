#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;
void UpdateDrawFrame(void);

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    return 0;
}

void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Sort Visualizer", 190, 200, 20, BLACK);
    EndDrawing();
}
