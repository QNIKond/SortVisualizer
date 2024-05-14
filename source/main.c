#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../external/raygui.h"
#include "SortConfig.h"
#include "SettingsTab.h"
#include "../external/darkTheme.h"
#include "SortVisualiser.h"
#include "SortProphiler.h"
#include "ArraySorter.h"

void UpdateDrawFrame(void);

SConfig input;
Image icon;

void LoadDrawIcon(){
    icon = GenImageColor(128,128,(Color){0});
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
    InitWindow(1200, 800, "Sort visualiser");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    LoadDrawIcon();
    GuiLoadStyleDark();

    InitSortConfig(&input);
    InitSortVisualizer();
    InitProphiler();
    ValidateSorts();
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    FreeSortVisualizer();
    FreeProphiler();
    UnloadImage(icon);
    return 0;
}

bool showFPS = false;
#define SETTABWIDTH 200
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground((Color) {0x2c,0x2c,0x2c,0xff});

    UpdateDrawSettingTab(&input, (Rectangle){0, 0, SETTABWIDTH, GetScreenHeight()});

    Rectangle mainFrame = (Rectangle){SETTABWIDTH, 0, GetScreenWidth() - SETTABWIDTH, GetScreenHeight()};
    if(input.visual.currentTab == 0) {
        SyncConfigsForVis(&input);
        UpdateDrawSortAnimation(mainFrame);
    }
    else {
        SyncConfigsForProph(&input);
        UpdateDrawProphiler(mainFrame);
    }
    
    if(IsKeyDown(KEY_F))
        showFPS = !showFPS;
    if(showFPS)
        DrawFPS(0,0);
    EndDrawing();
}
