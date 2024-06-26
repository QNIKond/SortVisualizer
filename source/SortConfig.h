#pragma once
#include "raylib.h"

#define LISTOFSORTS \
    SMACRO(Insertion)       \
    SMACRO(Shell)           \
    SMACRO(Bubble)          \
    SMACRO(Shaker)          \
    SMACRO(Heap)



typedef enum {
#define SMACRO(X) SA##X##Sort,
LISTOFSORTS
#undef SMACRO
SANone //Must be lust
    } SortingAlgorithm;

typedef enum {
    RandomShuffle,
    SlightShuffle
} ShufflingAlgorithm;

typedef enum {
    Bars,
    Pyramid,
    Circle
} Visualisation;

typedef enum {
    PlainColoring,
    RainbowColoring,
    DisparityColoring,
    GradientColoring,
} Coloring;

typedef enum {
    MTRealTime = 0,
    MTCPUTime
}MeasureTarget;

typedef struct{
    struct {
        int size;
        SortingAlgorithm sortingAlgorithm;
        ShufflingAlgorithm shufflingAlgorithm;
        bool updated;
    } array;
    struct {
        Visualisation visualisation;
        bool disparity;
        bool showValueAsLength;
        bool hollow;
        bool connectedDots;
        bool mirrored;
        Coloring coloring;
        Color col1;
        Color col2;
        int animationLength;
        bool showShuffling;
        bool showProgressBars;
        bool showInfo;

        bool isOnPause;
        int currentTab;

    } visual;
    struct {
        SortingAlgorithm sortingAlgorithms[SANone];
        int saCount;
        ShufflingAlgorithm shufflingAlgorithm;
        int minSize;
        int maxSize;
        MeasureTarget measureTarget;
        int nCount;
        int average;
        int threads;
#define MAXTHREADS 32

        bool atStart;
        bool updated;
    } proph;

    struct {
        Color graphColors[SANone];
        bool staticY;
        bool blackWhiteMode;
    } graph;

    bool runBtn;
    bool resetBtn;
    bool isRunning;
    bool isProphiling;
} SConfig;

void InitSortConfig(SConfig *sconfig);

//int SyncConfigsForVis(SConfig *back, SConfig *front, bool fixed);