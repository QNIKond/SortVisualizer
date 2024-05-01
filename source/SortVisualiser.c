#include "SortVisualiser.h"

#define SVBARSVOFFSET 100
#define SVBARSHOFFSET 100

void DrawBars(Rectangle bounds, SConfig *sconf, InputArray *input){
    int width = (bounds.width-SVBARSHOFFSET*2)/sconf->arraySize;
    double height = (bounds.height-SVBARSVOFFSET*2)/sconf->arraySize;
    for(int i = 0; i < sconf->arraySize; ++i){
        DrawRectangle(bounds.x+SVBARSHOFFSET+width*i, bounds.y+bounds.height-SVBARSVOFFSET-height*(input->arr[i] + 1),
                      width, height*(input->arr[i] + 1), sconf->col1);
    }
}

void DrawArray(Rectangle bounds, SConfig *sconf, InputArray *input){
    switch(sconf->visualisation){
        case Bars:
            DrawBars(bounds,sconf,input);
            break;
        case Pyramid:
            break;
        case Circle:
            break;
    }
}