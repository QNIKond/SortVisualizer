#pragma once
#include "InputArray.h"

typedef struct{
    //All
    int i;
    bool isSorted;

    //Shaker, Insertion
    int leftBoundary;
    int rightBoundary;
    int direction;

    //Shaker
    int unreplacementCounter;

    //Shell sort
    int stepS;
    int stepI;
    int stepJ;
    bool shellSorted;
} SortData;

int StepSortArray(SortingAlgorithm alg, InputArray *input, SortData *data);

void SortArray(SortingAlgorithm alg, InputArray *input);

void ResetSorter(SortData *data, int arraySize);

int EstimateSorter(SortingAlgorithm alg, InputArray *input, InputArray *sorted);

void ValidateSorts();