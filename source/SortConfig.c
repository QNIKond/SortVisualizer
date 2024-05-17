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
    sconfig->proph.maxSize = 20000;
    sconfig->proph.nCount = 20;
    sconfig->proph.threads = 6;
    sconfig->graph.staticY = false;
    sconfig->graph.graphColors[0] = BLUE;
    sconfig->graph.graphColors[1] = GREEN;
    sconfig->graph.graphColors[2] = RED;
    sconfig->graph.graphColors[3] = YELLOW;
    for(int i = 4; i < SANone; ++i)
        sconfig->graph.graphColors[i] = WHITE;
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