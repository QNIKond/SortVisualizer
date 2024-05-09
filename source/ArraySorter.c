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

int StepSortArray(SConfig *sconf, InputArray *input, SortData *data){
    switch (sconf->array.sortingAlgorithm) {

        case BubbleSort:
            return StepBubbleSort(input, data);
        case ShakerSort:
            return 0;
        case GravitySort:
            return 0;
    }
}

void ResetSorter(SortData *data, int arraySize){
    *data = (SortData){0};
    data->isSorted = true;
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