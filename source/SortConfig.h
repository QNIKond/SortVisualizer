#pragma once
#include "raylib.h"

typedef enum {
    InsertionSort = 0,
    ShellSort,
    BubbleSort,
    ShakerSort,
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
    GradientColoring,
    RainbowColoring
} Coloring;

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
        SortingAlgorithm sortingAlgorithm;
        ShufflingAlgorithm shufflingAlgorithm;
        int minSize;
        int maxSize;
        int nCount;

        bool updated;
    } proph;

    struct {
        Color col1;
        bool showDots;
    } graph;

    bool runBtn;
    bool resetBtn;
    bool isRunning;
    bool isProphiling;
} SConfig;

void InitSortConfig(SConfig *sconfig);

//int SyncConfigsForVis(SConfig *back, SConfig *front, bool fixed);