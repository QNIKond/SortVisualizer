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

void LoadIcon(){
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
    LoadIcon();
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
/*
int sortTime = 0;
double leftOver = 0;
int sortFramesLeft = 0;

int GetCurFramesCount(){
    int left = backEnd.vs.animationLength*60 - sortTime;
    double perFrame = (double)EstimateSorter(&backEnd,&arr,&sorted) / left;
    int toDoe = (int)perFrame;
    leftOver += perFrame - toDoe;
    toDoe += (int)leftOver;
    leftOver -= (int)leftOver;
    return toDoe;
}
*/
int algFrames;
int animFrames;
int algCount;
int animCount;
#define SHUFFLEDURATION 1.5
void UpdateDrawFrame(void)
{

    BeginDrawing();
    ClearBackground((Color) {0x2c,0x2c,0x2c,0xff});
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
            algFrames = EstimateShuffler(&backEnd);
            animFrames = SHUFFLEDURATION * 60;
        }
        algCount = 0;
        animCount = 0;
    }

    if(!backEnd.vs.isOnPause) {
        if (backEnd.animState == AnimShuffling) {
            if(animFrames-animCount-1)
                ++animCount;
            while((backEnd.animState == AnimShuffling) &&
            ((((double)(algFrames-algCount)/(animFrames-animCount)) >= ((double)algCount/animCount))||
                    (animFrames-animCount-1 <= 0))) {
                ++algCount;
                if (StepShuffleArray(&backEnd, &arr)) {
                    backEnd.animState = AnimSorting;
                    frontEnd.animState = AnimSorting;
                    algFrames = EstimateSorter(&backEnd,&arr,&sorted);
                    animFrames = backEnd.vs.animationLength*60;
                    algCount = 0;
                    animCount = 1;
                }
            }

        } else if (backEnd.animState == AnimSorting) {
            if(backEnd.vs.animationLength*60-animCount-1)
                ++animCount;
            while((backEnd.animState == AnimSorting) &&
                  ((((double)(algFrames-algCount)/(backEnd.vs.animationLength*60-animCount)) >= ((double)algCount/animCount))||
                  (backEnd.vs.animationLength*60-animCount-1 <= 0))) {
                ++algCount;
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
