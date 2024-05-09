#pragma once
#include "raylib.h"
#include "SortConfig.h"
#include "InputArray.h"

void SyncConfigs(SConfig *input);

void UpdateDrawSortAnimation(Rectangle bounds);

void InitSortVisualizer();

void FreeSortVisualizer();