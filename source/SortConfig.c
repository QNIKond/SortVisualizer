#include "SortConfig.h"

void InitSortConfig(SConfig *sconfig){
    *sconfig = (SConfig){0};
    sconfig->array.size = 40;
    sconfig->visual.showValueAsLength = true;
    sconfig->visual.coloring = GradientColoring;
    sconfig->visual.col1 = WHITE;
    sconfig->visual.col2 = RED;
    sconfig->visual.animationLength = 15;
    sconfig->visual.showShuffling = true;
    sconfig->visual.showProgressBars = true;
    sconfig->array.sortingAlgorithm = SABubbleSort;
    sconfig->proph.sortingAlgorithms[0] = SABubbleSort;
    for(int i = 1; i < SANone; ++i)
        sconfig->proph.sortingAlgorithms[i] = SANone;
    sconfig->proph.saCount = 1;
    sconfig->proph.minSize = 100;
    sconfig->proph.maxSize = 100000;
    sconfig->proph.nCount = 100;
    sconfig->proph.threads = 6;
    sconfig->graph.staticY = false;
    sconfig->graph.col1 = BLUE;
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