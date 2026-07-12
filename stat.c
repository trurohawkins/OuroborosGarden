#include "stat.h"

Stat empty = {
	.id = EMPTY,
	.value = EMPTY
};

Nub *initStats(Form *f, int numStats) {
	Nub *stat = growNub(f);
	stat->type = NUB;
	stat->owned = true;
	Stat *stats = calloc(numStats + 1, sizeof(Stat));
	stats[0].id = numStats + 1;
	for (int i = 1; i < stats[0].id; i++) {
		stats[i] = empty;
	}
	stat->data = stats;
	return stat;
}

bool addStat(Form *f, int id, float value) {
	Nub *stat = findNub(f, NUB);
	if (stat) {
		Stat *stats = stat->data;
		for (int i = 1; i < stats[0].id; i++) {
			if (stats[i].id == EMPTY) {
				stats[i].id = id;
				stats[i].value = value;
				return true;
			}
		}
	}
	return false;
}

float *getStat(Form *f, int id) {
	Nub *stat = findNub(f, NUB);
	if (stat) {
		Stat *stats = stat->data;
		for (int i = 1; i < stats[0].id; i++) {
			if (stats[i].id != EMPTY && stats[i].id == id) {
				return &stats[i].value;
			}
		}
	}
	return 0;
}

bool setStat(Form *f, int id, float value) {
	float *stat = getStat(f, id);
	if (stat) {
		*stat = value;
		return true;
	} else {
		return false;
	}
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
