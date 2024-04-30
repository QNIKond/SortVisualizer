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
}InputArray;

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
    SortingAlgorithm sortingAlgorithm;
    InputArray inputArray;
    int *array;
    int arraySize;
    int arrayModifier;
    ShufflingAlgorithm shufflingAlgorithm;
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
    bool currentTab;
} SConfig;

void InitializeSortConfig(SConfig *sconfig);