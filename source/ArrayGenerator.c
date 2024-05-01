#include "ArrayGenerator.h"
#include "malloc.h"

void GenerateLinear(SConfig *sconf,InputArray *input){
    for(int i = 0; i < sconf->as.arraySize; ++i)
        input->arr[i] = i;

}

void UpdateInputArray(SConfig *sconf, InputArray *input){
    if(!input->arr) {
        input->arr = calloc(sconf->as.arraySize, sizeof(int));
        input->size = sconf->as.arraySize;
    }
    else if(input->size<sconf->as.arraySize) {
        input->arr = realloc(input->arr, sconf->as.arraySize*sizeof(int));
        input->size = sconf->as.arraySize;
    }
    switch(sconf->as.inputArrayFunction){

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