#include "ArraySorter.h"
#include "stdlib.h"
#include "stdio.h"

void BubbleSort(InputArray *input) {
    for (int i = 0; i < input->filled - 1; i++) {
        for (int j = (input->filled - 1); j > i; j--) {
            if (input->arr[j - 1] > input->arr[j]) {
                int t = input->arr[j - 1];
                input->arr[j - 1] = input->arr[j];
                input->arr[j] = t;
            }
        }
    }
}

int StepBubbleSort(InputArray *input, SortData *data) {
    if (data->i + 1 >= input->filled) {
        if (data->isSorted)
            return 1;
        data->isSorted = true;
        data->i = 0;
    }
    if (input->arr[data->i] > input->arr[data->i + 1]) {
        int t = input->arr[data->i];
        input->arr[data->i] = input->arr[data->i + 1];
        input->arr[data->i + 1] = t;
        data->isSorted = false;
    }
    ++data->i;
    return 0;
}

void ShakerSort(InputArray *input) {
    int leftBoundary = 0;
    int rightBoundary = input->filled - 2;
    bool wasReplacement = true;

    while ((leftBoundary <= rightBoundary) && wasReplacement) {
        wasReplacement = false;

        for (int i = rightBoundary; i >= leftBoundary; i--) {
            if (input->arr[i] > input->arr[i + 1]) {
                int t = input->arr[i];
                input->arr[i] = input->arr[i + 1];
                input->arr[i + 1] = t;
                wasReplacement = true;
            }
        }
        leftBoundary++;

        for (int i = leftBoundary; i <= rightBoundary; i++) {
            if (input->arr[i] > input->arr[i + 1]) {
                int t = input->arr[i];
                input->arr[i] = input->arr[i + 1];
                input->arr[i + 1] = t;

                wasReplacement = true;
            }
        }
        rightBoundary--;
    }
}

int StepShakerSort(InputArray *input, SortData *data) {
    if (data->isSorted) {
        data->isSorted = false;
    }
    if (data->leftBoundary >= data->rightBoundary ||
        data->unreplacementCounter >= data->rightBoundary - data->leftBoundary) {
        data->isSorted = true;
        data->i = 0;
        return 1;
    }
    if (data->i <= data->rightBoundary - 1 && data->i >= data->leftBoundary) {
        if (input->arr[data->i] > input->arr[data->i + 1]) {
            int t = input->arr[data->i];
            input->arr[data->i] = input->arr[data->i + 1];
            input->arr[data->i + 1] = t;
            data->unreplacementCounter = 0;
        } else {
            ++data->unreplacementCounter;
        }
    } else {
        data->direction *= -1;
        data->unreplacementCounter = 0;
    }
    if (data->i > data->rightBoundary) {
        --data->rightBoundary;
        --data->i;
    }
    if (data->i < data->leftBoundary) {
        ++data->leftBoundary;
        ++data->i;
    }
    data->i += data->direction;
    return 0;
}

void InsertionSort(InputArray *input) {
    for (int i = 1; i < input->filled; i++) {
        int j = i - 1;
        int t = input->arr[i];
        while ((input->arr[j] > t) && (j >= 0)) {
            input->arr[j + 1] = input->arr[j];
            j--;
        }
        input->arr[j + 1] = t;
    }
}

int StepInsertionSort(InputArray *input, SortData *data) {
    //data->direction - temp element
    //data->leftBoundary - j counter
    if (data->isSorted) {
        data->isSorted = false;
        data->i = 1;
        data->leftBoundary = -2;
    }

    if (data->i >= input->filled) {
        return 1;
    }

    if (data->leftBoundary == -2) {
        data->leftBoundary = data->i - 1;
        data->direction = input->arr[data->i];
    }

    if ((input->arr[data->leftBoundary] > data->direction) && (data->leftBoundary >= 0)) {
        input->arr[data->leftBoundary + 1] = input->arr[data->leftBoundary];
        data->leftBoundary--;
    } else {
        data->i++;
        input->arr[data->leftBoundary + 1] = data->direction;
        data->leftBoundary = -2;
    }

    return 0;
}

void ShellSort(InputArray *input) {
    for (int s = input->filled / 2; s > 0; s /= 2) {
        for (int i = s; i < input->filled; ++i) {
            for (int j = i - s; j >= 0 && input->arr[j] > input->arr[j + s]; j -= s) {
                int t = input->arr[j];
                input->arr[j] = input->arr[j + s];
                input->arr[j + s] = t;
            }
        }
    }
}

int SteShellSort(InputArray *input, SortData *data) {
    for (int s = input->filled / 2; s > 0; s /= 2) {
        for (int i = s; i < input->filled; ++i) {
            for (int j = i - s; j >= 0 && input->arr[j] > input->arr[j + s]; j -= s) {
                int t = input->arr[j];
                input->arr[j] = input->arr[j + s];
                input->arr[j + s] = t;
            }
        }
    }
    return 1;
}

int StepShellSort(InputArray *input, SortData *data){
    if(data->shellSorted)
        return 1;

    if((data->stepJ < 0) || (input->arr[data->stepJ] <= input->arr[data->stepJ + data->stepS])){
        ++data->stepI;
        if(data->stepI >= input->filled){
            data->stepS /= 2;
            if(data->stepS <= 0) {
                data->shellSorted = true;
                return 1;
            }
            data->stepI = data->stepS;
        }
        data->stepJ = data->stepI - data->stepS;
    }
    else{
        int t = input->arr[data->stepJ];
        input->arr[data->stepJ] = input->arr[data->stepJ + data->stepS];
        input->arr[data->stepJ + data->stepS] = t;
        data->stepJ -= data->stepS;
    }
    return 0;
}

int StepSortArray(SortingAlgorithm alg, InputArray *input, SortData *data) {
    switch (alg) {
        #define SMACRO(X) case SA##X##Sort: return Step##X##Sort(input,data);
        LISTOFSORTS
        #undef SMACRO
    }
    return 1;
}

void SortArray(SortingAlgorithm alg, InputArray *input) {
    switch (alg) {
        #define SMACRO(X) case SA##X##Sort:  X##Sort(input); break;
        LISTOFSORTS
        #undef SMACRO
    }
}

void ResetSorter(SortData *data, int arraySize) {
    *data = (SortData) {.i = 0, .leftBoundary = 0, .rightBoundary = arraySize - 1};
    data->isSorted = true;
    data->direction = 1;
    data->unreplacementCounter = 0;

    data->stepS = arraySize / 2;
    data->stepI = arraySize / 2;
    data->stepJ = 0;
    data->shellSorted = false;
}

int EstimateSorter(SortingAlgorithm alg, InputArray *input, InputArray *sorted) {
    SortData data;
    ResetSorter(&data, input->filled);
    ResizeInputArray(sorted, input->filled);
    for (int i = 0; i < input->filled; ++i) {
        sorted->arr[i] = input->arr[i];
    }
    int counter = 0;
    while (!StepSortArray(alg, sorted, &data))
        ++counter;
    return ++counter;
}

#define TESTARRSIZE 128
#define MAXSTEPSCOUNT 200000

int IsSorted(InputArray *arr){
    for(int i = 0; i < arr->filled-1; ++i)
        if(arr->arr[i] > arr->arr[i+1])
            return 0;
    return 1;
}

void TestStepArray(int (*stepAlg)(InputArray*,SortData*), InputArray *arr, const char* sortName, const char* arrName){
    SortData sortData;
    int i = 0;
    ResetSorter(&sortData, TESTARRSIZE);
    while(!stepAlg(arr,&sortData)) {
        ++i;
        if (i > MAXSTEPSCOUNT)
            break;
    }
    if(!IsSorted(arr))
        printf("Step %s sort is not working for %s array\n", sortName, arrName);
}

void ValidateSorts(){
    void (*alg[])(InputArray*) = {
            #define SMACRO(X) X##Sort,
            LISTOFSORTS
            #undef SMACRO
    };
    int (*stepAlg[])(InputArray*,SortData*) = {
            #define SMACRO(X) Step##X##Sort,
            LISTOFSORTS
            #undef SMACRO
    };
    char names[][20] = {
            #define SMACRO(X) #X,
            LISTOFSORTS
            #undef SMACRO
    };
    InputArray revA;
    InputArray randA;
    InputArray sortedA;
    InitInputArray(&revA,TESTARRSIZE);
    InitInputArray(&randA,TESTARRSIZE);
    InitInputArray(&sortedA,TESTARRSIZE);
    for(int i = 0; i < sizeof(alg)/sizeof(alg[0]); ++i){
        for(int j = 0; j < TESTARRSIZE; ++j){
            revA.arr[j] = TESTARRSIZE - j - 1;
            randA.arr[j] = rand()%TESTARRSIZE;
        }
        alg[i](&revA);
        if(!IsSorted(&revA))
            printf("%s is not working for reverse array\n", names[i]);
        alg[i](&randA);
        if(!IsSorted(&revA))
            printf("%s is not working for random array\n", names[i]);
        alg[i](&sortedA);
        if(!IsSorted(&revA))
            printf("%s is not working for sorted array\n", names[i]);
    }

    for(int i = 0; i < sizeof(alg)/sizeof(alg[0]); ++i){
        for(int j = 0; j < TESTARRSIZE; ++j){
            revA.arr[j] = TESTARRSIZE - j - 1;
            randA.arr[j] = rand()%TESTARRSIZE;
        }
        TestStepArray(stepAlg[i],&revA,names[i],"reverse");
        TestStepArray(stepAlg[i],&randA,names[i],"random");
        TestStepArray(stepAlg[i],&sortedA,names[i],"sorted");
    }

    FreeInputArray(&revA);
    FreeInputArray(&randA);
    FreeInputArray(&sortedA);
}