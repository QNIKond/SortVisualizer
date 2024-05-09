#include "InputArray.h"
#include "malloc.h"

void GenerateArray(InputArray *input){
    for(int i = 0; i < input->filled; ++i)
        input->arr[i] = i;
}

void InitInputArray(InputArray *input, int size){
    input->arr = calloc(size, sizeof(int));
    input->size = size;
    input->filled = size;
    GenerateArray(input);
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