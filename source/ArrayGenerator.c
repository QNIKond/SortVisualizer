#include "ArrayGenerator.h"
#include "malloc.h"

void GenerateLinear(SConfig *sconf,InputArray *input){
    for(int i = 0; i < sconf->as.arraySize; ++i)
        input->arr[i] = i;
    input->maxElement = sconf->as.arraySize;
}

void GenerateArray(SConfig *sconf, InputArray *input){
    switch(sconf->as.inputArrayFunction){

        case LinearArray:
            GenerateLinear(sconf,input);
            break;
        case SqareRootArray:
            break;
    }
}

void InitInputArray(SConfig *sconf, InputArray *input){
    input->arr = calloc(sconf->as.arraySize, sizeof(int));
    input->size = sconf->as.arraySize;
    GenerateArray(sconf,input);
}

void UpdateInputArray(SConfig *sconf, InputArray *input){
    if(input->size<sconf->as.arraySize) {
        input->arr = realloc(input->arr, sconf->as.arraySize*sizeof(int));
        input->size = sconf->as.arraySize;
    }
    GenerateArray(sconf,input);
}

void FreeInputArray(InputArray *arr){
    free(arr->arr);
    arr->arr = 0;
}