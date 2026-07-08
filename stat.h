#pragma once
#include "FormEngine.h"

#define EMPTY -42069
#define NUB 3

Nub * initStats(Form *f, int numStats);
bool addStat(Form *f, int id, float value);
float *getStat(Form *f, int stat);

Form *checkStat(int x, int y, int id);
