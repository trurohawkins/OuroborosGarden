#include "water.h"

Form *water = 0;
linkedList *body = 0;

void initWater() {
	if (!water) {
		water = makeForm(WATER);
		initStats(water, 1);
		addStat(water, SOURCE, 0.05f);
		Nub *ren = growRenderNub(water);
		RenderObject *rob = ren->data;
		rob->data = water;
		rob->render= renderWater;
	}
}

void placeWater(int x, int y) {
	int *pos = calloc(2, sizeof(int));
	pos[0] = x;
	pos[1] = y;
	addToList(&body, pos);
	placeForm(water, x, y);
}

void *renderWater(void *data) {
	Form *water = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = DIRTLAYER,
	};

	linkedList *cur = body;
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
	freeList(&body);
}
