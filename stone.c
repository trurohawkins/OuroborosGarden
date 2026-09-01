#include "stone.h"

Mass *stone = 0;

void initStone() {
	if (!stone) {
		stone = makeMass(STONE);
		initStats(stone->self, 2);
		addStat(stone->self, COVER, 0.05f);
		addStat(stone->self, BLOCK, 1);
		Nub *ren = growRenderNub(stone->self);
		RenderObject *rob = ren->data;
		rob->data = stone;
		rob->render= renderStone;
	}
}

Form *placeStone(int x, int y) {
	return addToMass(stone, x, y);
}

void *renderStone(void *data) {
	Mass *stone = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = GRASSLAYER,
	};
	Color color = {
		.rgb = {
			60, 80, 70
		},
	};
	memcpy(reco.data, &color, sizeof(Color));

	linkedList *cur = stone->body;
	while (cur) {
		int *pos = cur->data;
		reco.pos.x = worldXToScreenX(pos[0]);
		reco.pos.y = worldYToScreenY(pos[1]);
		addRenderCommand(reco);
		cur = cur->next;
	}
}

void freeStone() {
	if (stone) {
		freeMass(stone);
		stone = 0;
	}
}

