#pragma once
#include "InputArray.h"

int ShuffleArray(SConfig *sconf, InputArray *input);

int StepShuffleArray(SConfig *sconf, InputArray *input);

void ResetShuffler();

int EstimateShuffler(SConfig *sconf);