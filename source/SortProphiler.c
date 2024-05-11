#include "SortProphiler.h"
#include "malloc.h"
#include "pthread.h"
#include "semaphore.h"

SConfig sc;
bool atStart = true;
int *graphData = 0;
int gdFilled = 0;
int gdMaxValue;
int gdSize = 0;
sem_t left;

void InitProphiler(){
    gdSize = 6;
    graphData = calloc(gdSize,sizeof(int));
    graphData[0] = 2;
    graphData[1] = 4;
    graphData[2] = 1;
    graphData[3] = 5;
    graphData[4] = 6;
    graphData[5] = 3;
    gdMaxValue = 6;
    gdFilled = gdSize;
}

void ResetProphiler(){

}
void StartSortingThreads(){
    /*if(gdSize<sc.proph.maxSize-sc.proph.minSize){
        gdSize = sc.proph.maxSize-sc.proph.minSize;
        graphData = realloc(graphData,gdSize);
    }
    sem_post_multiple(&left,gdSize);*/
}

#define DOTRADIUS 4
#define LINETHICKNESS 2
void DrawGraph(Rectangle bounds){
    if(!graphData)
        return;
    Vector2 prevDot = {0};
    float stepX = bounds.width/(gdFilled-1);
    float stepY = bounds.height/gdMaxValue;
    for(int i = 0; i < gdFilled; ++i){
        Vector2 dot = {i*stepX+bounds.x,bounds.height-graphData[i]*stepY+bounds.y};
        if(sc.graph.showDots)
            DrawCircleV(dot,DOTRADIUS,sc.graph.col1);
        if(prevDot.x)
            DrawLineEx(prevDot,dot,LINETHICKNESS,sc.graph.col1);
        prevDot = dot;
    }
}

#define PROPHPADDING 100
void UpdateDrawProphiler(Rectangle bounds){
    bounds.x += PROPHPADDING;
    bounds.y += PROPHPADDING;
    bounds.width -= 2 * PROPHPADDING;
    bounds.height -= 2 * PROPHPADDING;
    DrawRectangleRec(bounds,BLACK);
    if(sc.resetBtn){
        if(atStart) {
            StartSortingThreads();
        }
        else
            ResetProphiler();
    }
    DrawGraph(bounds);
}

void SyncConfigsForProph(SConfig *input){
    if(atStart){
        sc.proph = input->proph;
    }
    sc.graph = input->graph;
}

void FreeProphiler(){
    free(graphData);
}