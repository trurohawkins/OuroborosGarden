#include "water.h"

Mass *water = 0;

void initWater() {
	if (!water) {
		water = makeMass(WATER);
		initStats(water->self, 1);
		addStat(water->self, SOURCE, 0.015f);
		Nub *ren = growRenderNub(water->self);
		RenderObject *rob = ren->data;
		rob->data = water;
		rob->render = renderWater;

		addTimedEvent(rain, 0, rainInterval);
	}
}

void rain(void *) {
	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y*w->x)+x];
			bool covered = false;
			for (int i = FORMS_PER_CELL-1; i >= 0; i--) {
				if (c->within[i]) {
					if (getStat(c->within[i], COVER)) {
						covered = true;
						break;
					}
				}
			}
			if (!covered) {
				addEco(x, y, rainAmount);
			}
		}
	}
}

Form *placeWater(int x, int y) {
	return addToMass(water, x, y);
}

void *renderWater(void *data) {
	Mass *water = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = DIRTLAYER,
	};
	Color color = {
		.rgb = {
			50, 100, 200
		}
	};
	memcpy(reco.data, &color, sizeof(Color));
	renderMass(data, reco);
}

void freeWater() {
	if (water) {
		freeMass(water);
		water = 0;
	}
}
