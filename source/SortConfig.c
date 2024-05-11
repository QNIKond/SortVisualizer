#include "SortConfig.h"

void InitSortConfig(SConfig *sconfig){
    *sconfig = (SConfig){0};
    sconfig->array.size = 40;
    sconfig->visual.showValueAsLength = true;
    sconfig->visual.col1 = WHITE;
    sconfig->visual.col2 = RED;
    sconfig->visual.animationLength = 15;
    sconfig->visual.showShuffling = true;
    sconfig->visual.showProgressBars = true;
    sconfig->array.sortingAlgorithm = BubbleSort;
    sconfig->proph.sortingAlgorithm = BubbleSort;
    sconfig->graph.showDots = true;
    sconfig->graph.col1 = RED;
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