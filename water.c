#include "water.h"

Form *water = 0;
linkedList *waterBody = 0;

void initWater() {
	if (!water) {
		water = makeForm(WATER);
		initStats(water, 1);
		addStat(water, SOURCE, 0.015f);
		Nub *ren = growRenderNub(water);
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
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		int *pos = calloc(2, sizeof(int));
		pos[0] = x;//(x % w->x + w->x) % w->x;
		pos[1] = y;//(y % w->y + w->y) % w->y;
		addToList(&waterBody, pos);
		placeForm(water, pos[0], pos[1]);
		return water;
	}
	return NULL;
}

void *renderWater(void *data) {
	//Form *water = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = DIRTLAYER,
	};

	linkedList *cur = waterBody;
	while (cur) {
		int *pos = cur->data;
		PosColor pc = {
			.pos = {
				.x = worldXToScreenX(pos[0]),
				.y = worldYToScreenY(pos[1]),
			},
			.color = {
				.vals = {
					50, 100, 200
				}
			},
		};
		memcpy(reco.data, &pc, sizeof(PosColor));
		addRenderCommand(reco);
		cur = cur->next;
	}
}

void freeWater() {
	if (waterBody) {
		linkedList *cur = waterBody;
		while (cur) {
			int *pos = cur->data;
			removeForm(water, pos[0], pos[1]);
			cur = cur->next;
		}
		freeList(&waterBody);
	}
	freeForm(water);
	waterBody = 0;
	water = 0;
}
