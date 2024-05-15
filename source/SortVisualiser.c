#include "SortVisualiser.h"
#include "InputArray.h"
#include "SortVisualiser.h"
#include "ArrayShuffler.h"
#include "ArraySorter.h"
#include "math.h"

InputArray arr;
InputArray sorted;
SConfig sconf;
SortData sortData;

int algFrames;
int algCount;
int animCount;
double prevAlgCount;

enum AnimState{
    AnimStart = 0,
    AnimShuffling,
    AnimSorting,
    AnimEnd
} animState;

#define SHUFFLEDURATION 1.5

#define SVBARSVOFFSET 100
#define SVBARSHOFFSET 100

Color GetVisColor(Vector2 pos, int max){
    float t;
    switch(sconf.visual.coloring){

        case PlainColoring:
            return sconf.visual.col1;
        case RainbowColoring:
            return ColorFromHSV(360.0f*pos.y/(float)max,1,1);
        case DisparityColoring:
            t = (float)abs((int)(pos.x-pos.y))/(float)max;
            t = sqrt(t) * 255;
            return ColorAlphaBlend(RED,BLUE,(Color){t,t,t,t});
        case GradientColoring:
            t = 255*pos.y/(float)max;
            return ColorAlphaBlend(RED,BLUE,(Color){t,t,t,t});
    }
}

void DrawBars(Rectangle bounds){
    double length = bounds.width-SVBARSHOFFSET*2;
    double height = (bounds.height-SVBARSVOFFSET*2)/arr.filled;
    if(arr.filled<length){
        double width =  length/arr.filled;
        for(int i = 0; i < arr.filled; ++i){
            DrawRectangle((int)(bounds.x+SVBARSHOFFSET+width*i), bounds.y+bounds.height-SVBARSVOFFSET-height*(arr.arr[i] + 1),
                          (int)width+1, height*(arr.arr[i] + 1),
                          GetVisColor((Vector2){i,arr.arr[i]},arr.filled));
        }
    }
    else{
        double iSkip =  arr.filled/length;
        for(int i = 0; i < length; ++i){
            DrawLine((int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET-height*(arr.arr[(int)(i*iSkip)] + 1),
                     (int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET,
                     GetVisColor((Vector2){(int)(i*iSkip),arr.arr[(int)(i*iSkip)]},arr.filled));
        }
    }
}

#define EMPTYPBCOLOR BLACK
#define FILLEDPBCOLOR BLUE
#define PBHEIGHT 16
void DrawProgressBar(Rectangle bounds){
    DrawRectangle(bounds.x,bounds.y+bounds.height-PBHEIGHT,bounds.width,PBHEIGHT,EMPTYPBCOLOR);
    float progress;
    if(animState == AnimStart)
        progress = 0;
    else if(animState == AnimEnd)
        progress = 1;
    else if(animState == AnimSorting)
        progress = (float)algCount / (float)algFrames;
    else
        progress = (float)animCount / (float)(SHUFFLEDURATION*60);
    DrawRectangle(bounds.x,bounds.y+bounds.height-PBHEIGHT, (int)(bounds.width * progress), PBHEIGHT, FILLEDPBCOLOR);
}

void DrawArray(Rectangle bounds){
    switch(sconf.visual.visualisation){
        case Bars:
            DrawBars(bounds);
            break;
        case Pyramid:
            break;
        case Circle:
            break;
    }
    DrawProgressBar(bounds);
}

void ResetSortVisualizer(){
    ResizeInputArray(&arr,sconf.array.size);
    GenerateArray(&arr);
    ResetShuffler();
    ResetSorter(&sortData, arr.filled);
    if(!sconf.visual.showShuffling)
        while(!StepShuffleArray(&sconf, &arr));
}

void InitSortVisualizer(){
    InitSortConfig(&sconf);
    InitInputArray(&arr,sconf.array.size);
    InitInputArray(&sorted,sconf.array.size);
    ResetSortVisualizer();
};

void FreeSortVisualizer(){
    FreeInputArray(&arr);
    FreeInputArray(&sorted);
}

void SwitchAnimState(enum AnimState state) {
    switch (state) {

        case AnimStart:
            animState = AnimStart;
            ResetSortVisualizer();
            break;
        case AnimShuffling:
            algFrames = EstimateShuffler(&sconf);
            animCount = 0;
            algCount = 0;
            prevAlgCount = 0;
            animState = AnimShuffling;
            break;
        case AnimSorting:
            algFrames = EstimateSorter(sconf.array.sortingAlgorithm,&arr,&sorted);
            animCount = 0;
            algCount = 0;
            prevAlgCount = 0;
            animState = AnimSorting;
            break;
        case AnimEnd:
            animState = AnimEnd;
            break;
    }
}

void SyncConfigsForVis(SConfig *input){
    if(animState == AnimStart) {
        input->visual.isOnPause = false;
        if (input->array.updated) {
            input->array.updated = false;
            sconf.array = input->array;
            ResetSortVisualizer();
        }
    }
    sconf.runBtn |= input->runBtn;
    sconf.resetBtn |= input->resetBtn;
    sconf.visual = input->visual;
    input->isRunning = (animState==AnimSorting) || (animState==AnimShuffling);
}

int GetCyclesCount(double animLength){
    animLength *= 60;
    int over = -(int)(animCount*((double)(algFrames)/(animLength)));
    ++animCount;
    over += (int)(animCount*((double)(algFrames)/(animLength)));
    return over;
}

void PlayAnimStart(){
    if(sconf.runBtn) {
        SwitchAnimState(AnimShuffling);
        sconf.runBtn = false;
    }
}

void PlayAnimShuffling(){
    if(!sconf.visual.showShuffling){
        while(!StepShuffleArray(&sconf, &arr));
        SwitchAnimState(AnimSorting);
        return;
    } else{
        for(int i = GetCyclesCount(SHUFFLEDURATION); i > 0; --i) {
            ++algCount;
            if (StepShuffleArray(&sconf, &arr)) {
                SwitchAnimState(AnimSorting);
                break;
            }
        }
    }
}

void PlayAnimSorting(){
    for(int i = GetCyclesCount(sconf.visual.animationLength); i > 0; --i) {
        ++algCount;
        if (StepSortArray(sconf.array.sortingAlgorithm, &arr, &sortData)) {
            SwitchAnimState(AnimEnd);
            break;
        }
    }
}

void PlayAnimEnd(){
    if(sconf.runBtn)
        SwitchAnimState(AnimStart);
}

void UpdateDrawSortAnimation(Rectangle bounds){
    if(sconf.resetBtn) {
        SwitchAnimState(AnimStart);
        sconf.resetBtn = false;
    }

    switch (animState) {
        case AnimStart:
            PlayAnimStart();
            break;
        case AnimShuffling:
            if(!sconf.visual.isOnPause)
                PlayAnimShuffling();
            break;
        case AnimSorting:
            if(!sconf.visual.isOnPause)
                PlayAnimSorting();
            break;
        case AnimEnd:
            PlayAnimEnd();
            break;
    }

    if((animState == AnimShuffling) || (animState == AnimSorting))
        DrawRectangleRec(bounds,(Color) {0x2c,0x2c,0x2c,0xff});
    else
        DrawRectangleRec(bounds,(Color) {0x28,0x28,0x28,0xff});
    DrawArray(bounds);
}