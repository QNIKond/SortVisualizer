#include "SortProphiler.h"
#include "InputArray.h"
#include "ArrayShuffler.h"
#include "malloc.h"
#include "pthread.h"
#include "semaphore.h"
#include "time.h"
#include "stdio.h"
#include "unistd.h"
#include "math.h"
#include "../external/raygui.h"

#define THREADSC 12

bool atStart = true;
#define TIMTEST CLOCK_PROCESS_CPUTIME_ID
SConfig sc;
InputArray arr1;
int *graphData = 0;
int gdSize = 0;
int gdFilled = 0;
int gdMaxValue;
double nStep;
pthread_mutex_t mutex;

typedef struct{
    InputArray arr;
    int step;
    int offset;
    int exit;
    int seed;
}ThreadInput;
pthread_rwlock_t tiLock;

pthread_t threads[THREADSC];
ThreadInput tInput[THREADSC];

void InitProphiler(){

    pthread_mutex_init(&mutex,NULL);
    pthread_rwlock_init(&tiLock,NULL);
    for(int i = 0; i < THREADSC; ++i)
        InitInputArray(&tInput[i].arr,1);
}

int Bubble(InputArray *arr){

    double res = ((double)arr->filled*(double)arr->filled)/(10000);
    usleep((int)res);
    return 0;
    //return (t%213)*473 %16;
}
void *SortT(void *inref){
    ThreadInput *in = (ThreadInput*)inref;
    srand(in->seed);
    int i = in->offset;
    while((i<gdFilled)) {
        pthread_rwlock_rdlock(&tiLock);
        if(in->exit) {
            pthread_rwlock_unlock(&tiLock);
            return NULL;
        }
        pthread_rwlock_unlock(&tiLock);
        in->arr.filled = (int)((double)i * nStep + sc.proph.minSize);
        GenerateArray(&in->arr);
        ShuffleArray(&sc,&in->arr);
        struct timespec start;
        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC,&start);
        Bubble(&in->arr);
        clock_gettime(CLOCK_MONOTONIC,&end);
        int time = (int)(end.tv_sec-start.tv_sec)*1000+(int)((end.tv_nsec-start.tv_nsec)*1e-6);
        pthread_mutex_lock(&mutex);
        graphData[i] = time;
        if(time > gdMaxValue)
            gdMaxValue = time;
        pthread_mutex_unlock(&mutex);
        i += in->step;
    }
    return NULL;
}

void StartSortingThreads(){
    pthread_rwlock_wrlock(&tiLock);
    for(int i = 0; i < THREADSC; ++i) {
        tInput[i].exit = 1;
    }
    pthread_rwlock_unlock(&tiLock);
    for(int i = 0; i < THREADSC; ++i){
        pthread_join(threads[i],NULL);
    }
    gdFilled = sc.proph.nCount;
    if(gdSize<gdFilled){
        gdSize = gdFilled;
        graphData = realloc(graphData,gdSize*sizeof(int));
    }
    for(int i = 0; i < gdFilled; ++i)
        graphData[i] = -1;
    gdMaxValue = 0;
    nStep = (double)(sc.proph.maxSize-sc.proph.minSize)/gdFilled;

    for(int i = 0; i < THREADSC; ++i) {
        ResizeInputArray(&(tInput[i].arr),sc.proph.maxSize);
        tInput[i].step = THREADSC;
        tInput[i].offset = i;
        tInput[i].exit = 0;
        tInput[i].seed = rand();
        pthread_create(&threads[i],NULL,SortT,(void*)&tInput[i]);

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
        if(graphData[i] == -1)
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

#define PROPHPADDING 120
void UpdateDrawProphiler(Rectangle bounds){
    bounds.x += PROPHPADDING;
    bounds.y += PROPHPADDING;
    bounds.width -= 2 * PROPHPADDING;
    bounds.height -= 2 * PROPHPADDING;

    if(sc.resetBtn){
        //clock_gettime(TIMTEST,&start);
        StartSortingThreads();
        sc.resetBtn = false;
    }
    if(gdSize)
        DrawGraph(bounds);
}

void SyncConfigsForProph(SConfig *input){
    if(atStart){
        sc.proph = input->proph;
    }
    sc.graph = input->graph;
    sc.runBtn |= input->runBtn;
    sc.resetBtn |= input->resetBtn;
}

void FreeProphiler(){
    if(graphData)
        free(graphData);
    pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&tiLock);
    for(int i = 0; i < THREADSC; ++i)
        FreeInputArray(&tInput[i].arr);
}