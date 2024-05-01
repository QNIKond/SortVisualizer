#include "ArrayShuffler.h"
#include "stdlib.h"

void ShuffleRandom(SConfig *sconf, InputArray *input){
    for(int i = 0; i < sconf->arraySize; ++i){
        int t = input->arr[i];
        int r = rand()%sconf->arraySize;
        input->arr[i] = input->arr[r];
        input->arr[r] = t;
    }
}

void ShuffleArray(SConfig *sconf, InputArray *input){
    UpdateInputArray(sconf,input);
    switch (sconf->shufflingAlgorithm) {

        case RandomShuffle:
            ShuffleRandom(sconf,input);
            break;
        case SlightShuffle:
            break;
    }
}