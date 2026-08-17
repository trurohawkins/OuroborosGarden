#include "water.h"

Form *water = 0;
linkedList *waterBody = 0;

void initWater() {
	if (!water) {
		water = makeForm(WATER);
		initStats(water, 1);
		addStat(water, SOURCE, 0.05f);
		Nub *ren = growRenderNub(water);
		RenderObject *rob = ren->data;
		rob->data = water;
		rob->render = renderWater;
	}
}

void placeWater(int x, int y) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		int *pos = calloc(2, sizeof(int));
		pos[0] = x;//(x % w->x + w->x) % w->x;
		pos[1] = y;//(y % w->y + w->y) % w->y;
		addToList(&waterBody, pos);
		placeForm(water, pos[0], pos[1]);
	}
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
				50, 100, 200
			},
		};
		memcpy(reco.data, &pc, sizeof(PosColor));
		addRenderCommand(reco);
		cur = cur->next;
	}
}

void freeWater() {
	freeList(&waterBody);
}
