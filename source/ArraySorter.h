#pragma once
#include "InputArray.h"

typedef struct{
    int i;
    int leftBoundary;
    int rightBoundary;
    bool isSorted;
    bool isRightDirection;
    bool isReplaced;
} SortData;

int StepSortArray(SConfig *sconf, InputArray *input, SortData *data);

void ResetSorter(SortData *data, int arraySize);

int EstimateSorter(SConfig *sconf, InputArray *input, InputArray *sorted);