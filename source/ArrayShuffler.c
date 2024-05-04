#include "ArrayShuffler.h"
#include "stdlib.h"

struct SUShared{
    int i;
} suShared;

int StepShuffleRandom(SConfig *sconf, InputArray *input){
    if(suShared.i >= input->filled)
        return 1;
    int t = input->arr[suShared.i];
    int r = rand()%input->filled;
    input->arr[suShared.i] = input->arr[r];
    input->arr[r] = t;
    ++suShared.i;
    return 0;
}

int StepShuffleArray(SConfig *sconf, InputArray *input){
    switch (sconf->as.shufflingAlgorithm) {

        case RandomShuffle:
            return StepShuffleRandom(sconf, input);
            break;
        case SlightShuffle:
            return 0;
    }
}

void ResetShufflers(){
    suShared = (struct SUShared){0};
}

int EstimateShuffler(SConfig *sconf){
    switch (sconf->as.shufflingAlgorithm) {

        case RandomShuffle:
            return sconf->as.arraySize;
        case SlightShuffle:
            return sconf->as.arraySize;
    }
}