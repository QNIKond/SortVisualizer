#pragma once
#include "InputArray.h"

typedef struct{
    int i;
    bool isSorted;
} SortData;

int StepSortArray(SConfig *sconf, InputArray *input, SortData *data);

void ResetSorter(SortData *data);

int EstimateSorter(SConfig *sconf, InputArray *input, InputArray *sorted);