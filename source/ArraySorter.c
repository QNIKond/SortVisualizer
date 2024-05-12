#include "ArraySorter.h"

int StepBubbleSort(InputArray *input, SortData *data){
    if(data->i + 1 >= input->filled) {
        if(data->isSorted)
            return 1;
        data->isSorted = true;
        data->i = 0;
    }
    if(input->arr[data->i] > input->arr[data->i + 1]) {
        int t = input->arr[data->i];
        input->arr[data->i] = input->arr[data->i + 1];
        input->arr[data->i + 1] = t;
        data->isSorted = false;
    }
    ++data->i;
    return 0;
}

void FullShakerSort(InputArray *input){
    int leftBoundary = 0;
    int rightBoundary = input->filled - 1;
    bool wasReplacement = true;

    while ((leftBoundary <= rightBoundary) && wasReplacement) {
        wasReplacement = false;

        for (int i = rightBoundary; i >= leftBoundary; i--) {
            if (input->arr[i] > input->arr[i + 1]) {
                int t = input->arr[i];
                input->arr[i] = input->arr[i + 1];
                input->arr[i + 1] = t;
                wasReplacement = true;
            }
        }
        leftBoundary++;

        for (int i = leftBoundary; i <= rightBoundary; i++) {
            if (input->arr[i] > input->arr[i + 1]) {
                int t = input->arr[i];
                input->arr[i] = input->arr[i + 1];
                input->arr[i + 1] = t;
                wasReplacement = true;
            }
        }
        rightBoundary--;
    }
}

int StepShakerSort(InputArray *input, SortData *data){
    if(data->isSorted){
        data->isSorted = false;
    }
    if(data->leftBoundary >= data->rightBoundary || data->unreplacementCounter >= data->rightBoundary - data->leftBoundary) {
       data->isSorted = true;
       data->i = 0;
       return 1;
    }
    if(data->i <= data->rightBoundary - 1 && data->i >= data->leftBoundary) {
        if(input->arr[data->i] > input->arr[data->i + 1]) {
            int t = input->arr[data->i];
            input->arr[data->i] = input->arr[data->i + 1];
            input->arr[data->i + 1] = t;
            data->unreplacementCounter = 0;
        } else {
            ++data->unreplacementCounter;
        }
    } else {
        data->direction *= -1;
        data->unreplacementCounter = 0;
    }
    if(data->i > data->rightBoundary) {
        --data->rightBoundary;
        --data->i;
    }
    if(data->i < data->leftBoundary) {
        ++data->leftBoundary;
        ++data->i;
    }
    data->i += data->direction;
    return 0;
}

int StepSortArray(SConfig *sconf, InputArray *input, SortData *data){
    switch (sconf->array.sortingAlgorithm) {
        case InsertionSort:
            break;
        case ShellSort:
            break;
        case BubbleSort:
            return StepBubbleSort(input, data);
        case ShakerSort:
            return StepShakerSort(input, data);
    }
    return 1;
}


void ResetSorter(SortData *data, int arraySize){
    *data = (SortData){.i = 0, .leftBoundary = 0, .rightBoundary = arraySize - 1};
    data->isSorted = true;
    data->direction = 1;
    data->unreplacementCounter = 0;
}

int EstimateSorter(SConfig *sconf, InputArray *input, InputArray *sorted){
    SortData data;
    ResetSorter(&data, input->filled);
    ResizeInputArray(sorted, input->filled);
    for(int i = 0; i < input->filled; ++i){
        sorted->arr[i] = input->arr[i];
    }
    int counter = 0;
    while(!StepSortArray(sconf,sorted, &data))
        ++counter;
    return ++counter;
}