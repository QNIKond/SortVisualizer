#include "SortConfig.h"

void InitializeSortConfig(SConfig *sconfig){
    *sconfig = (SConfig){0};
    sconfig->as.arraySize = 40;
    sconfig->vs.showValueAsLength = true;
    sconfig->vs.col1 = WHITE;
    sconfig->vs.col2 = RED;
    sconfig->vs.animationLength = 15;
    sconfig->vs.showShuffling = true;
    sconfig->vs.showProgressBars = true;
    sconfig->isInInitState = true;
}

bool isEqual(char *a, char *b, int size){
    for(int i = 0; i < size; ++i)
        if(*(a+i) != *(b+i))
            return false;
    return true;
}

int SyncConfigs(SConfig *back, SConfig *front){
    back->vs = front->vs;
    if(!isEqual((char*)&back->as,(char*)&front->as,sizeof(front->as))) {
        if (back->isInInitState) {
            back->needsReloading = false;
            back->as = front->as;
            return 1;
        } else
            back->needsReloading = true;
    }
    return 0;
}