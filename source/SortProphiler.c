#include "SortProphiler.h"
#include "InputArray.h"
#include "ArrayShuffler.h"
#include "malloc.h"
#include "pthread.h"
#include "semaphore.h"
#include "time.h"

#define THREADSC 24

bool atStart = true;

SConfig sc;
InputArray arr1;
int *graphData = 0;
int gdSize = 0;
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
    long long t;
    time(&t);
    int test = rand()%16;
    return test;
    //return (t%213)*473 %16;
}
void *SortT(void *inref){
    ThreadInput *in = (ThreadInput*)inref;
    srand(in->seed);
    int i = in->offset;
    while((i<sc.proph.nCount)) {
        pthread_rwlock_rdlock(&tiLock);
        if(in->exit) {
            pthread_rwlock_unlock(&tiLock);
            return NULL;
        }
        pthread_rwlock_unlock(&tiLock);
        in->arr.filled = (int)((double)i * nStep + sc.proph.minSize);
        GenerateArray(&in->arr);
        ShuffleArray(&sc,&in->arr);
        int time = Bubble(&in->arr);
        pthread_mutex_lock(&mutex);
        graphData[i] = time;
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

    if(gdSize<sc.proph.nCount){
        gdSize = sc.proph.nCount;
        graphData = realloc(graphData,gdSize*sizeof(int));
    }
    for(int i = 0; i < gdSize; ++i)
        graphData[i] = 0;
    gdMaxValue = 16;
    nStep = (double)(sc.proph.maxSize-sc.proph.minSize)/sc.proph.nCount;

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
void DrawGraph(Rectangle bounds){
    pthread_mutex_lock(&mutex);
    if(!graphData) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    Vector2 prevDot = {0};
    float stepX = bounds.width/(sc.proph.nCount-1);
    float stepY = bounds.height/gdMaxValue;
    for(int i = 0; i < sc.proph.nCount; ++i){
        Vector2 dot = {i*stepX+bounds.x,bounds.height-graphData[i]*stepY+bounds.y};
        if(sc.graph.showDots)
            DrawCircleV(dot,DOTRADIUS,sc.graph.col1);
        if(prevDot.x)
            DrawLineEx(prevDot,dot,LINETHICKNESS,sc.graph.col1);
        prevDot = dot;
    }
    pthread_mutex_unlock(&mutex);
}

#define PROPHPADDING 100
void UpdateDrawProphiler(Rectangle bounds){
    bounds.x += PROPHPADDING;
    bounds.y += PROPHPADDING;
    bounds.width -= 2 * PROPHPADDING;
    bounds.height -= 2 * PROPHPADDING;
    DrawRectangleRec(bounds,BLACK);
    if(sc.resetBtn){
        StartSortingThreads();
        sc.resetBtn = false;
    }
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