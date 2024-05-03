#include "SortVisualiser.h"

#define SVBARSVOFFSET 100
#define SVBARSHOFFSET 100

void DrawBars(Rectangle bounds, SConfig *sconf, InputArray *input){
    double length = bounds.width-SVBARSHOFFSET*2;
    double height = (bounds.height-SVBARSVOFFSET*2)/input->maxElement;
    if(sconf->as.arraySize<length){
        double width =  length/sconf->as.arraySize;
        for(int i = 0; i < sconf->as.arraySize; ++i){
            DrawRectangle((int)(bounds.x+SVBARSHOFFSET+width*i), bounds.y+bounds.height-SVBARSVOFFSET-height*(input->arr[i] + 1),
                          (int)width+1, height*(input->arr[i] + 1), sconf->vs.col1);
        }
    }
    else{
        double iSkip =  sconf->as.arraySize/length;
        for(int i = 0; i < length; ++i){
            DrawLine((int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET-height*(input->arr[(int)(i*iSkip)] + 1),
                     (int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET, sconf->vs.col1);
        }
    }
}

void DrawArray(Rectangle bounds, SConfig *sconf, InputArray *input){
    switch(sconf->vs.visualisation){
        case Bars:
            DrawBars(bounds,sconf,input);
            break;
        case Pyramid:
            break;
        case Circle:
            break;
    }
}

