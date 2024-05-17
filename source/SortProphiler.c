#include "SortProphiler.h"
#include "InputArray.h"
#include "ArrayShuffler.h"
#include "ArraySorter.h"
#include "malloc.h"
#include "pthread.h"
#include "semaphore.h"
#include "time.h"
#include "stdio.h"
#include "unistd.h"
#include "math.h"
#include "../external/raygui.h"



//#define THREADSC 6




SConfig sc;
int *graphData = 0;
int gdSize = 0;
int gdFilled = 0;
int gdMaxValue;
double nStep;
int clockType;
pthread_mutex_t mutex;
pthread_attr_t pattr;

struct TEXIT{
    bool shouldExit;
    int activeThreads;
    pthread_mutex_t exitMutex;
} TExit = {
        .shouldExit = false,
        .activeThreads = 0
};

typedef struct{
    InputArray arr;
    int step;
    int offset;
    int seed;
}ThreadInput;
pthread_rwlock_t tiLock;

pthread_t threads[MAXTHREADS];
ThreadInput tInput[MAXTHREADS];

#define PSEMPTY -1
#define PSRESERVED -2

void InitProphiler(){

    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&TExit.exitMutex,NULL);
    pthread_rwlock_init(&tiLock,NULL);
    pthread_attr_init(&pattr);
    pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_DETACHED);
    for(int i = 0; i < MAXTHREADS; ++i) {
        InitInputArray(&tInput[i].arr, 1);
    }
    sc.proph.atStart = true;
}

int ReserveNext(int *i, int step, int prevResult){
    pthread_mutex_lock(&mutex);
    if(prevResult>=0) {
        graphData[*i] = prevResult;
        if (prevResult > gdMaxValue)
            gdMaxValue = prevResult;
    }
    while((*i < gdFilled) && (graphData[*i] != PSEMPTY))
        *i += step;
    if(*i < gdFilled)
        graphData[*i] = PSRESERVED;
    pthread_mutex_unlock(&mutex);
    return *i < gdFilled;
}

void *SortT(void *inref){
    ThreadInput *in = (ThreadInput*)inref;
    srand(in->seed);
    int i = in->offset;
    int result = PSEMPTY;
    while(ReserveNext(&i,in->step,result)) {
        in->arr.filled = (int)((double)i * nStep + sc.proph.minSize);
        GenerateArray(&in->arr);
        ShuffleArray(&sc,&in->arr);
        struct timespec start;
        struct timespec end;
        clock_gettime(clockType,&start);
        SortArray(sc.proph.sortingAlgorithms[0], &in->arr);
        clock_gettime(clockType,&end);
        result = (int)(end.tv_sec-start.tv_sec)*1000+(int)((end.tv_nsec-start.tv_nsec)*1e-6);
    }
    pthread_mutex_lock(&TExit.exitMutex);
    --TExit.activeThreads;
    pthread_mutex_unlock(&TExit.exitMutex);
    return NULL;
}



void StartSortingThreads(){
    gdFilled = sc.proph.nCount;
    switch(sc.proph.measureTarget){
        case MTRealTime:
            clockType = CLOCK_MONOTONIC;
            break;
        case MTCPUTime:
            clockType = CLOCK_THREAD_CPUTIME_ID;
            break;
    }
    if(gdSize<gdFilled){
        gdSize = gdFilled;//20
        graphData = realloc(graphData,gdSize*sizeof(int));
    }
    for(int i = 0; i < gdFilled; ++i)
        graphData[i] = PSEMPTY;
    gdMaxValue = 0;
    nStep = (double)(sc.proph.maxSize-sc.proph.minSize)/gdFilled;

    TExit.activeThreads = sc.proph.threads;
    TExit.shouldExit = false;
    for(int i = 0; i < TExit.activeThreads; ++i) {
        ResizeInputArray(&(tInput[i].arr),sc.proph.maxSize);
        tInput[i].step = TExit.activeThreads;
        tInput[i].offset = i;
        tInput[i].seed = rand();
        pthread_create(&threads[i],&pattr,SortT,(void*)&tInput[i]);
    }
}

#define DOTRADIUS 4
#define LINETHICKNESS 2
#define AXISTHICKNESS 4
#define BGCOLOR (Color){0x24,0x24,0x24,0xff}
#define AXISCOLOR GRAY
#define SUBAXISCOLOR DARKGRAY
#define MINSUBDIVHEIGHT 40
#define MINSUBDIVWIDTH 80

void DrawGraphBack(Rectangle bounds){
    DrawRectangleRec(bounds,BGCOLOR);
    DrawLineEx((Vector2) {bounds.x, bounds.y},(Vector2) {bounds.x, bounds.y + bounds.height},AXISTHICKNESS, AXISCOLOR);
    DrawLineEx((Vector2) {bounds.x, bounds.y+bounds.height},(Vector2) {bounds.x+bounds.width, bounds.y + bounds.height},AXISTHICKNESS, AXISCOLOR);
    GuiLabel((Rectangle) {bounds.x - 90, bounds.y - 35, 180, 24},"Execution time (ms)");
    GuiLabel((Rectangle) {bounds.x+bounds.width, bounds.y+bounds.height-12, 100, 24},"Array size");
    double r = bounds.height/gdMaxValue;
    double vdivy;
    if(!sc.graph.staticY) {
        double t = pow(10, floor(log10(MINSUBDIVHEIGHT / r)));
        vdivy = round(MINSUBDIVHEIGHT / r * 2 / t) * t / 2;
        if (vdivy < 1)
            vdivy = 1;
    }
    else{
        vdivy = gdMaxValue/(bounds.height/MINSUBDIVHEIGHT);
    }
    int i = 0;
    while(i * vdivy * r <= bounds.height+1){
        int y;
        if(!sc.graph.staticY)
            y = bounds.y+bounds.height- i * vdivy * r;
        else
            y = bounds.y+bounds.height- MINSUBDIVHEIGHT*i;
        char val[20];
        itoa(i*vdivy,val,10);
        GuiLabel((Rectangle){bounds.x-40,y-12,40,24},val);
        DrawLine(bounds.x,y, bounds.x + bounds.width, y, SUBAXISCOLOR);
        ++i;
    }
    int hsdCount = bounds.width/MINSUBDIVWIDTH;
    for(int i = 1; i <= hsdCount; ++i){
        Vector2 pnt = {bounds.x + MINSUBDIVWIDTH*i-2,bounds.y+bounds.height-5};
        DrawRectangle(pnt.x-2,pnt.y-5,4,10,SUBAXISCOLOR);
        char val[20];
        itoa(sc.proph.maxSize/hsdCount*i,val,10);
        GuiLabel((Rectangle){pnt.x-30,pnt.y+12,60,24},val);
    }
}

void DrawGraph(Rectangle bounds){
    DrawGraphBack(bounds);
    pthread_mutex_lock(&mutex);
    if(!graphData) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    Vector2 prevDot = {0};
    float stepX = bounds.width/(gdFilled-1);
    float stepY = bounds.height/gdMaxValue;
    for(int i = 0; i < gdFilled; ++i){
        if(graphData[i] < 0)
            break;
        Vector2 dot = {i*stepX+bounds.x,bounds.height-graphData[i]*stepY+bounds.y};
        if(gdFilled<=50)
            DrawCircleV(dot,DOTRADIUS,sc.graph.col1);
        if(prevDot.x) {
            DrawLineEx(prevDot, dot, LINETHICKNESS, sc.graph.col1);
        }
        prevDot = dot;
    }
    pthread_mutex_unlock(&mutex);
}

int GetActiveThreads() {
    pthread_mutex_lock(&TExit.exitMutex);
    int result = TExit.activeThreads;
    pthread_mutex_unlock(&TExit.exitMutex);
    return result;
}

void StopThreads(){
    pthread_mutex_lock(&TExit.exitMutex);
    TExit.shouldExit = true;
    pthread_mutex_unlock(&TExit.exitMutex);
}

#define PROPHPADDING 120
void UpdateDrawProphiler(Rectangle bounds){
    bounds.x += PROPHPADDING;
    bounds.y += PROPHPADDING;
    bounds.width -= 2 * PROPHPADDING;
    bounds.height -= 2 * PROPHPADDING;
    if(!sc.proph.atStart && !GetActiveThreads())
        sc.proph.atStart = true;
    if(sc.resetBtn){
        if(sc.proph.atStart) {
            StartSortingThreads();
            sc.proph.atStart = false;
        }
        else
            StopThreads();
        sc.resetBtn = false;
    }

    if(gdSize)
        DrawGraph(bounds);
}

void SyncConfigsForProph(SConfig *input){
    input->proph.atStart = sc.proph.atStart;
    if(sc.proph.atStart){
        input->proph.updated = false;
        sc.proph = input->proph;
    }
    sc.graph = input->graph;
    sc.runBtn |= input->runBtn;
    sc.resetBtn |= input->resetBtn;
}

void FreeProphiler(){
    StopThreads();
    if(graphData)
        free(graphData);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&TExit.exitMutex);
    pthread_rwlock_destroy(&tiLock);
    pthread_attr_destroy(&pattr);
    for(int i = 0; i < MAXTHREADS; ++i)
        FreeInputArray(&tInput[i].arr);
}