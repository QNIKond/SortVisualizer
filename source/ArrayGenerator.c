#include "ArrayGenerator.h"
#include "malloc.h"

void GenerateLinear(SConfig *sconf,InputArray *input){
    for(int i = 0; i < input->filled; ++i)
        input->arr[i] = i;
    input->maxElement = input->filled-1;
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
    input->filled = input->size;
    input->maxElement = 0;
    GenerateArray(sconf,input);
}

void ResizeInputArray(InputArray *input, int newSize){
    if(input->size<newSize) {
        input->arr = realloc(input->arr, newSize*sizeof(int));
        input->size = newSize;
    }
    input->filled = newSize;
}

void FreeInputArray(InputArray *arr){
    free(arr->arr);
    arr->arr = 0;
}