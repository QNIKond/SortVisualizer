#include "SortConfig.h"

void InitializeSortConfig(SConfig *sconfig){
    *sconfig = (SConfig){0};
    sconfig->arraySize = 40;
    sconfig->showValueAsLength = true;
    sconfig->col1 = WHITE;
    sconfig->col2 = RED;
    sconfig->animationLength = 15;
    sconfig->showShuffling = true;
    sconfig->showProgressBars = true;
}