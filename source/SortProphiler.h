#pragma once
#include "SortConfig.h"

void InitProphiler();

void SyncConfigsForProph(SConfig *input);

void UpdateDrawProphiler(Rectangle bounds);

void FreeProphiler();