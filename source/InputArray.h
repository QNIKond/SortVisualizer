#pragma once
#include "SortConfig.h"

typedef struct{
    int *arr;
    int filled;
    int size;
} InputArray;

void InitInputArray(SConfig *sconf, InputArray *input);

void GenerateArray(SConfig *sconf, InputArray *input);

void ResizeInputArray(InputArray *input, int newSize);

void FreeInputArray(InputArray *arr);