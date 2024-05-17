#include "ArrayShuffler.h"
#include "stdlib.h"

struct SUShared{
    int i;
} suShared;

int ShuffleArray(SConfig *sconf, InputArray *input){
    for(int i = 0; i < input->filled-1; ++i){//100
        int t = input->arr[i];
        int r = rand()%(input->filled-i-1)+i+1;
        input->arr[i] = input->arr[r];
        input->arr[r] = t;
    }
}

int StepShuffleRandom(SConfig *sconf, InputArray *input){
    if(suShared.i >= input->filled-1)
        return 1;
    int t = input->arr[suShared.i];
    int r = rand()%(input->filled-suShared.i-1)+suShared.i+1;
    input->arr[suShared.i] = input->arr[r];
    input->arr[r] = t;
    ++suShared.i;
    return 0;
}

int StepShuffleArray(SConfig *sconf, InputArray *input){
    switch (sconf->array.shufflingAlgorithm) {

        case RandomShuffle:
            return StepShuffleRandom(sconf, input);
            break;
        case SlightShuffle:
            return 0;
    }
}

void ResetShuffler(){
    suShared = (struct SUShared){0};
}

int EstimateShuffler(SConfig *sconf){
    switch (sconf->array.shufflingAlgorithm) {

        case RandomShuffle:
            return sconf->array.size;
        case SlightShuffle:
            return sconf->array.size;
    }
}