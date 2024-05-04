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
    sconfig->needsReloading = true;
}

int SyncConfigs(SConfig *back, SConfig *front){
    bool shouldReload = false;
    back->vs = front->vs;
    back->currentTab = front->currentTab;
    if(front->as.updated)
        back->needsReloading = true;
    if((back->needsReloading == true) && (front->animState == AnimStart)){
        back->needsReloading = false;
        front->as.updated = false;
        back->as = front->as;
        front->needsReloading = back->needsReloading;
        shouldReload = true;
    }
    front->needsReloading = back->needsReloading;
    if(((front->animState == AnimStart) && (back->animState != AnimStart)) ||
            ((front->animState == AnimShuffling) && !ANIMRUNNING(back->animState))){
        back->animState = front->animState;
        shouldReload = true;
    }
    return shouldReload;
}
//front back
// st shuf  |
// st sort  | - reset
// st end   |

// shuf st    - run
// shuf sort
// shuf end   - run

// sort st
// sort shuf
// sort end

// end shuf
// end sort
// end st