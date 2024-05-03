#pragma once
#include "raylib.h"
typedef enum {
    BubbleSort,
    ShakerSort,
    GravitySort
    } SortingAlgorithm;

typedef enum {
    LinearArray,
    SqareRootArray
}InputArrayFunction;

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
    GradientColoring,
    RainbowColoring
} Coloring;
#define ANIMLENMIN 2
#define ANIMLENMAX 100
typedef struct{
    struct {
        SortingAlgorithm sortingAlgorithm;
        InputArrayFunction inputArrayFunction;
        int arraySize;
        int arrayModifier;
        ShufflingAlgorithm shufflingAlgorithm;

        bool updated;
    } as;
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
    } vs;
    bool currentTab;
    bool needsReloading;
    /*bool isInInitState;
    bool isOver;
    bool isOnPause;*/
    enum {
        AnimStart = 0,
        AnimShuffling,
        AnimSorting,
        AnimEnd
    } animState;
} SConfig;

void InitializeSortConfig(SConfig *sconfig);

int SyncConfigs(SConfig *back, SConfig *front);