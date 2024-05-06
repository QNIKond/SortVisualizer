#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"
#include "SortConfig.h"
#include "SettingsTab.h"
#include "../darkTheme.h"
#include "SortVisualiser.h"
#include "ArrayShuffler.h"
#include "ArraySorter.h"

int screenWidth = 1200;
int screenHeight = 800;
void UpdateDrawFrame(void);

SConfig frontEnd;
SConfig backEnd;
InputArray arr = {0};
InputArray sorted = {0};
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
    InitWindow(screenWidth, screenHeight, "Sort visualiser");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    LoadDrawIcon();
    GuiLoadStyleDark();

    InitializeSortConfig(&frontEnd);
    InitializeSortConfig(&backEnd);
    InitInputArray(&frontEnd, &arr);
    InitInputArray(&frontEnd, &sorted);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    UnloadImage(icon);
    FreeInputArray(&arr);
    FreeInputArray(&sorted);
    return 0;
}

bool showFPS = false;
SortData sortData;

#define SHUFFLEDURATION 1.5
void UpdateDrawFrame(void)
{
    int t = 0x28;
    BeginDrawing();
    if(ANIMRUNNING(backEnd.animState))
        ClearBackground((Color) {0x2c,0x2c,0x2c,0xff});
    else
        ClearBackground((Color) {t,t,t,0xff});
    UpdateDrawSettingTab(&frontEnd, (Rectangle){0, 0, 200, GetScreenHeight()});
    if(SyncConfigs(&backEnd,&frontEnd)) {
        ResetShufflers();
        ResetSorter(&sortData);
        ResizeInputArray(&arr,backEnd.as.arraySize);
        GenerateArray(&backEnd,&arr);
        if(!backEnd.vs.showShuffling){
            while(!StepShuffleArray(&backEnd, &arr));
            /*backEnd.animState = AnimSorting;
            frontEnd.animState = AnimSorting;
            algFrames = EstimateSorter(&backEnd,&arr,&sorted);
            animFrames = backEnd.vs.animationLength*60;*/
        }else {
            backEnd.algFrames = EstimateShuffler(&backEnd);
            backEnd.animFrames = SHUFFLEDURATION * 60;
        }
        backEnd.algCount = 0;
        backEnd.animCount = 0;
    }

    if(!backEnd.vs.isOnPause) {
        if (backEnd.animState == AnimShuffling) {
            if(backEnd.animFrames-backEnd.animCount-1)
                ++backEnd.animCount;
            while((backEnd.animState == AnimShuffling) &&
            ((((double)(backEnd.algFrames-backEnd.algCount)/(backEnd.animFrames-backEnd.animCount)) >=
            ((double)backEnd.algCount/backEnd.animCount))||
                    (backEnd.animFrames-backEnd.animCount-1 <= 0))) {
                ++backEnd.algCount;
                if (StepShuffleArray(&backEnd, &arr)) {
                    backEnd.animState = AnimSorting;
                    frontEnd.animState = AnimSorting;
                    backEnd.algFrames = EstimateSorter(&backEnd,&arr,&sorted);
                    backEnd.animFrames = backEnd.vs.animationLength*60;
                    backEnd.algCount = 0;
                    backEnd.animCount = 1;
                }
            }

        } else if (backEnd.animState == AnimSorting) {
            if(backEnd.vs.animationLength*60-backEnd.animCount-1)
                ++backEnd.animCount;
            while((backEnd.animState == AnimSorting) &&
                  ((((double)(backEnd.algFrames-backEnd.algCount)/(backEnd.vs.animationLength*60-backEnd.animCount)) >=
                  ((double)backEnd.algCount/backEnd.animCount))||
                  (backEnd.vs.animationLength*60-backEnd.animCount-1 <= 0))) {
                ++backEnd.algCount;
                if (StepSortArray(&backEnd, &arr, &sortData)) {
                    backEnd.animState = AnimEnd;
                    frontEnd.animState = AnimEnd;
                }
            }
        }
    }
    DrawArray((Rectangle){200,0,GetScreenWidth()-200,GetScreenHeight()}, &backEnd, &arr);
    if(IsKeyDown(KEY_F))
        showFPS = !showFPS;
    if(showFPS)
        DrawFPS(0,0);
    EndDrawing();
}
