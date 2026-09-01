#include "clouds.h"

Mass *clouds = 0;

void initClouds() {
	if (!clouds) {
		clouds = makeMass(CLOUDS);
		Nub *ren = growRenderNub(clouds->self);
		RenderObject *rob = ren->data;
		rob->data = clouds;
		rob->render = renderClouds;
		addTimedEvent(rain, 0, rainInterval);
	}
}

Form *placeClouds(int x, int y) {
	return addToMass(clouds, x, y);
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

void *renderClouds(void *data) {
	Mass *clouds = data;
	RenderCommand reco = {
		.type = 3,
	};
	Tint tint = {
		.rgb = {
			0, 0, 0,
		},
		.value = -40,
		.strength = 0,
	};
	memcpy(reco.data, &tint, sizeof(Tint));
	renderMass(data, reco);
}

void freeClouds() {
	if (clouds) {
		freeMass(clouds);
		clouds = 0;
	}
}

