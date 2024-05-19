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

SConfig sc;
long long *graphData[SANone] = {0};
int gdSize[SANone] = {0};
int gdFilled = 0;
long long gdMaxValue;
double nStep;
int clockType;
pthread_spinlock_t gdSpin;
pthread_attr_t pattr;

struct TEXIT{
    bool shouldExit;
    int activeThreads;
    //pthread_rwlock_t exitLock;
    pthread_spinlock_t exitSpin;
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

pthread_t threads[MAXTHREADS];
ThreadInput tInput[MAXTHREADS];

#define PSEMPTY -1
#define PSRESERVED -2

void InitProphiler(){
    pthread_spin_init(&gdSpin,0);
    //pthread_mutex_init(&mutex,NULL);
    //pthread_rwlock_init(&TExit.exitLock,NULL);
    pthread_spin_init(&TExit.exitSpin,0);
    pthread_attr_init(&pattr);
    pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_DETACHED);
    for(int i = 0; i < MAXTHREADS; ++i) {
        InitInputArray(&tInput[i].arr, 1);
    }
    sc.proph.atStart = true;
}

int ReserveNext(int *i, int *j, int step, long long prevResult){
    pthread_spin_lock(&gdSpin);
    if(prevResult>=0) {
        graphData[*j][*i] = prevResult;
        if (prevResult > gdMaxValue)
            gdMaxValue = prevResult;
    }
    while((*i < gdFilled) && (graphData[*j][*i] != PSEMPTY)) {
        while((*j < sc.proph.saCount) && (graphData[*j][*i] != PSEMPTY))
            *j += 1;
        if(*j >= sc.proph.saCount) {
            *i += step;
            *j = 0;
        }
    }
    if(*i < gdFilled)
        graphData[*j][*i] = PSRESERVED;
    pthread_spin_unlock(&gdSpin);
    return *i < gdFilled;
}

void *SortT(void *inref){
    ThreadInput *in = (ThreadInput*)inref;
    srand(in->seed);
    int i = in->offset;
    int j = 0;
    long long result = PSEMPTY;
    while(ReserveNext(&i, &j,in->step,result)) {
        result = 0;
        for(int av = 0; av < sc.proph.average; ++av) {
            in->arr.filled = (int) ((double) i * nStep + sc.proph.minSize);
            GenerateArray(&in->arr);
            ShuffleArray(&sc, &in->arr);
            struct timespec start;
            struct timespec end;
            clock_gettime(clockType, &start);
            SortArray(sc.proph.sortingAlgorithms[j], &in->arr);
            clock_gettime(clockType, &end);
            result += (long long)(end.tv_sec - start.tv_sec) * 1e9 + (int) ((end.tv_nsec - start.tv_nsec));
        }
        result /= sc.proph.average;
    }
    //pthread_rwlock_wrlock(&TExit.exitLock);
    pthread_spin_lock(&TExit.exitSpin);
    --TExit.activeThreads;
    //pthread_rwlock_unlock(&TExit.exitLock);
    pthread_spin_unlock(&TExit.exitSpin);
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
    for(int i = 0; i < sc.proph.saCount; ++i)
        if(gdSize[i]<gdFilled){
            gdSize[i] = gdFilled;
            graphData[i] = realloc(graphData[i],gdSize[i]*sizeof(long long));
        }
    for(int i = 0; i < sc.proph.saCount; ++i)
        for(int j = 0; j < gdFilled; ++j)
            graphData[i][j] = PSEMPTY;
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
    double r = bounds.height/(gdMaxValue/1000000);
    double vdivy;
    if(!sc.graph.staticY) {
        double t = pow(10, floor(log10(MINSUBDIVHEIGHT / r)));
        vdivy = round(MINSUBDIVHEIGHT / r * 2 / t) * t / 2;
        if (vdivy < 1)
            vdivy = 1;
    }
    else{
        vdivy = (gdMaxValue/1000000)/(bounds.height/MINSUBDIVHEIGHT);
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
    for(int j = 1; j <= hsdCount; ++j){
        Vector2 pnt = {bounds.x + MINSUBDIVWIDTH*j-2,bounds.y+bounds.height-5};
        DrawRectangle(pnt.x-2,pnt.y-5,4,10,SUBAXISCOLOR);
        char val[20];
        itoa(sc.proph.maxSize/hsdCount*j,val,10);
        GuiLabel((Rectangle){pnt.x-30,pnt.y+12,60,24},val);
    }
}

void DrawGraph(Rectangle bounds, int graph){
    pthread_spin_lock(&gdSpin);
    if(!graphData[graph]) {
        pthread_spin_unlock(&gdSpin);
        return;
    }
    Vector2 prevDot = {0};
    float stepX = bounds.width/(gdFilled-1);
    float stepY = bounds.height/gdMaxValue;
    for(int i = 0; i < gdFilled; ++i){
        if(graphData[graph][i] < 0)
            break;
        Vector2 dot = {i*stepX+bounds.x,bounds.height-graphData[graph][i]*stepY+bounds.y};
        if(gdFilled<=50)
            DrawCircleV(dot,DOTRADIUS,sc.graph.graphColors[graph]);
        if(prevDot.x) {
            DrawLineEx(prevDot, dot, LINETHICKNESS, sc.graph.graphColors[graph]);
        }
        prevDot = dot;
    }
    pthread_spin_unlock(&gdSpin);
}

int GetActiveThreads() {
    //pthread_rwlock_rdlock(&TExit.exitLock);
    pthread_spin_lock(&TExit.exitSpin);
    int result = TExit.activeThreads;
    pthread_spin_unlock(&TExit.exitSpin);
    //pthread_rwlock_unlock(&TExit.exitLock);
    return result;
}

void StopThreads(){
    //pthread_rwlock_wrlock(&TExit.exitLock);
    pthread_spin_lock(&TExit.exitSpin);
    TExit.shouldExit = true;
    pthread_spin_unlock(&TExit.exitSpin);
    //pthread_rwlock_unlock(&TExit.exitLock);
}

void DrawGraphs(Rectangle bounds){
    DrawGraphBack(bounds);
    if(gdFilled) {
        for(int i = 0; i < sc.proph.saCount; ++i)
            DrawGraph(bounds, i);
    }
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
    DrawGraphs(bounds);
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
    while(GetActiveThreads());
    for(int i = 0; i < SANone; ++i)
        if(graphData[i])
            free(graphData[i]);
    pthread_spin_destroy(&gdSpin);
    //pthread_rwlock_destroy(&TExit.exitLock);
    pthread_spin_destroy(&TExit.exitSpin);
    pthread_attr_destroy(&pattr);
    for(int i = 0; i < MAXTHREADS; ++i)
        FreeInputArray(&tInput[i].arr);
}