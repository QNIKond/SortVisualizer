#pragma once
#include "InputArray.h"

typedef struct{
    int i;
    bool isSorted;
    int leftBoundary;
    int rightBoundary;
    int direction;
    int unreplacementCounter;
} SortData;

int StepSortArray(SConfig *sconf, InputArray *input, SortData *data);

void ResetSorter(SortData *data, int arraySize);

int EstimateSorter(SConfig *sconf, InputArray *input, InputArray *sorted);

void FullBubbleSort(InputArray *input);

void FullShakerSort(InputArray *input);

void FullInsertionSort(InputArray *input);