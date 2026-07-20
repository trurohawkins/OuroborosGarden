#pragma once
#include "FormEngine.h"

#define EMPTY -42069

typedef struct {
	int id;
	float value;
} Stat;

Nub * initStats(Form *f, int numStats);
bool addStat(Form *f, int id, float value);
float *getStat(Form *f, int stat);
bool setStat(Form *f, int stat, float value);

Form *checkStat(int x, int y, int id);
