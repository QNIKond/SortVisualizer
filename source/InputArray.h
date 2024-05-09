#pragma once
#include "SortConfig.h"

typedef struct{
    int *arr;
    int filled;
    int size;
} InputArray;

void InitInputArray(InputArray *input, int size);

void GenerateArray(InputArray *input);

void ResizeInputArray(InputArray *input, int newSize);

void FreeInputArray(InputArray *arr);