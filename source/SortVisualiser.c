#include "SortVisualiser.h"
#include "InputArray.h"
#define SVBARSVOFFSET 100
#define SVBARSHOFFSET 100

void DrawBars(Rectangle bounds, SConfig *sconf, InputArray *input){
    double length = bounds.width-SVBARSHOFFSET*2;
    double height = (bounds.height-SVBARSVOFFSET*2)/input->filled;
    if(input->filled<length){
        double width =  length/input->filled;
        for(int i = 0; i < input->filled; ++i){
            DrawRectangle((int)(bounds.x+SVBARSHOFFSET+width*i), bounds.y+bounds.height-SVBARSVOFFSET-height*(input->arr[i] + 1),
                          (int)width+1, height*(input->arr[i] + 1), sconf->vs.col1);
        }
    }
    else{
        double iSkip =  input->filled/length;
        for(int i = 0; i < length; ++i){
            DrawLine((int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET-height*(input->arr[(int)(i*iSkip)] + 1),
                     (int)(bounds.x+SVBARSHOFFSET+i), bounds.y+bounds.height-SVBARSVOFFSET, sconf->vs.col1);
        }
    }
}

#define EMPTYPBCOLOR BLACK
#define FILLEDPBCOLOR BLUE
#define PBHEIGHT 16

void DrawProgressBar(Rectangle bounds, SConfig *sconf){
    DrawRectangle(bounds.x,bounds.y+bounds.height-PBHEIGHT,bounds.width,PBHEIGHT,EMPTYPBCOLOR);
    float progress = (float)sconf->animCount / (float)sconf->animFrames;
    DrawRectangle(bounds.x,bounds.y+bounds.height-PBHEIGHT, (int)(bounds.width * progress), PBHEIGHT, FILLEDPBCOLOR);
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
    DrawProgressBar(bounds,sconf);
}