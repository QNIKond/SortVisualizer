#include "ArrayGenerator.h"
#include "malloc.h"

void GenerateLinear(SConfig *sconf,InputArray *input){
    for(int i = 0; i < sconf->arraySize; ++i)
        input->arr[i] = i;

}

void UpdateInputArray(SConfig *sconf, InputArray *input){
    if(!input->arr) {
        input->arr = calloc(sconf->arraySize, sizeof(InputArray));
        input->size = sconf->arraySize;
    }
    else if(input->size<sconf->arraySize) {
        input->arr = realloc(input->arr, sconf->arraySize);
        input->size = sconf->arraySize;
    }
    switch(sconf->inputArrayFunction){

        case LinearArray:
            GenerateLinear(sconf,input);
            break;
        case SqareRootArray:
            break;
    }
}

void FreeInputArray(InputArray *arr){
    free(arr->arr);
    arr->arr = 0;
}