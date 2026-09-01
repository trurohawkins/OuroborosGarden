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
