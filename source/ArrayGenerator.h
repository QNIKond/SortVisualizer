#pragma once
#include "SortConfig.h"

typedef struct{
    int *arr;
    int size;
    int maxElement;
} InputArray;

void InitInputArray(SConfig *sconf, InputArray *input);

void UpdateInputArray(SConfig *sconf, InputArray *input);

void FreeInputArray(InputArray *arr);