#include "stat.h"

Nub *initStats(Form *f, int numStats) {
	Nub *stat = growNub(f);
	stat->type = NUB;
	stat->owned = true;
	float *stats = calloc(numStats + 1, sizeof(float));
	stats[0] = numStats + 1;
	for (int i = 1; i < stats[0]; i++) {
		stats[i] = EMPTY;
	}
	stat->data = stats;
	return stat;
}

bool addStat(Form *f, int id, float value) {
	Nub *stat = findNub(f, NUB);
	if (stat) {
		float *stats = stat->data;
		if (id >= 0 && id < stats[0]) {
			if (stats[id] == EMPTY) {
				stats[id] = value;
				return true;
			}
		}
	}
	return false;
}

float *getStat(Form *f, int id) {
	Nub *stat = findNub(f, NUB);
	if (stat) {
		float *stats = stat->data;
		if (id >= 0 && id < stats[0]) {
			return &stats[id];
		}
	}
	return 0;
}

Form *checkStat(int x, int y, int id) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		Cell *c = &w->map[(y*theWorld.x) + x];
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			float *stat = getStat(c->within[i], id);
			if (stat) {
				return c->within[i];
			}
		}
	}
}
