#include "ArrayShuffler.h"
#include "stdlib.h"

struct SUShared{
    int i;
} suShared;

int StepShuffleRandom(SConfig *sconf, InputArray *input){
    int t = input->arr[suShared.i];
    int r = rand()%sconf->as.arraySize;
    input->arr[suShared.i] = input->arr[r];
    input->arr[r] = t;
    ++suShared.i;
    if(suShared.i >= sconf->as.arraySize)
        return 1;
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