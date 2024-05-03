#include "ArraySorter.h"

struct SOShared{
    int i;
    bool isSorted;
} soShared;

int StepBubbleSort(SConfig *sconf, InputArray *input){
    if(input->arr[soShared.i] > input->arr[soShared.i + 1]) {
        int t = input->arr[soShared.i];
        input->arr[soShared.i] = input->arr[soShared.i + 1];
        input->arr[soShared.i + 1] = t;
        soShared.isSorted = false;
    }
    ++soShared.i;
    if(soShared.i+1 >= sconf->as.arraySize) {
        if(soShared.isSorted)
            return 1;
        soShared.isSorted = true;
        soShared.i = 0;
    }
    return 0;
}

int StepSortArray(SConfig *sconf, InputArray *input){
    switch (sconf->as.sortingAlgorithm) {

        case BubbleSort:
            return StepBubbleSort(sconf,input);
        case ShakerSort:
            return 0;
        case GravitySort:
            return 0;
    }
}

void ResetSorters(){
    soShared = (struct SOShared){0};
}