#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"
#include "SortConfig.h"
#include "SettingsTab.h"
#include "../darkTheme.h"

int screenWidth = 1200;
int screenHeight = 800;
void UpdateDrawFrame(void);

SConfig sc;
Image icon;

void LoadIcon(){
    icon = GenImageColor(128,128,(Color){0});//(Color) {0x2c,0x2c,0x2c,0xff}
    ImageDrawCircle(&icon,64,64,64,BLACK);
    ImageDrawCircle(&icon,64,64,62,(Color) {0x2c,0x2c,0x2c,0xff});
    ImageDrawRectangle(&icon,24,90,18,14,(Color){0xc3,0xc3,0xc3,0xff});
    ImageDrawRectangle(&icon,44,70,18,34,(Color){0xc3,0xc3,0xc3,0xff});
    ImageDrawRectangle(&icon,64,50,18,54,(Color){0xc3,0xc3,0xc3,0xff});
    ImageDrawRectangle(&icon,84,30,20,74,(Color){0xc3,0xc3,0xc3,0xff});
    SetWindowIcon(icon);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Sort visualiser");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    LoadIcon();


    GuiLoadStyleDark();
    InitializeSortConfig(&sc);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    UnloadImage(icon);
    return 0;
}

void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground((Color) {0x2c,0x2c,0x2c,0xff});
    UpdateDrawSettingTab(&sc,(Rectangle){0,0,200,GetScreenHeight()});
    EndDrawing();
}
