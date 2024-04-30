#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"
#include "SortConfig.h"
#include "SettingsTab.h"

int screenWidth = 1200;
int screenHeight = 800;
void UpdateDrawFrame(void);

SConfig sc;

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);


    InitializeSortConfig(&sc);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    return 0;
}

void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    UpdateDrawSettingTab(&sc,(Rectangle){0,0,200,GetScreenHeight()});
    EndDrawing();
}
