#include "stone.h"

Form *stone = 0;
linkedList *stoneBody = 0;

void initStone() {
	if (!stone) {
		stone = makeForm(STONE);
		initStats(stone, 1);
		addStat(stone, COVER, 0.05f);
		Nub *ren = growRenderNub(stone);
		RenderObject *rob = ren->data;
		rob->data = stone;
		rob->render= renderStone;
	}
}

void placeStone(int x, int y) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		int *pos = calloc(2, sizeof(int));
		pos[0] = x;//(x % w->x + w->x) % w->x;
		pos[1] = y;//(y % w->y + w->y) % w->y;
		addToList(&stoneBody, pos);
		placeForm(stone, pos[0], pos[1]);
	}
}

void *renderStone(void *data) {
	Form *stone = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = GRASSLAYER,
	};

	linkedList *cur = stoneBody;
	while (cur) {
		int *pos = cur->data;
		PosColor pc = {
			.pos = {
				.x = worldXToScreenX(pos[0]),
				.y = worldYToScreenY(pos[1]),
			},
			.color = {
				60, 80, 70
			},
		};
		memcpy(reco.data, &pc, sizeof(PosColor));
		addRenderCommand(reco);
		cur = cur->next;
	}
}

void freeStone() {
	if (stoneBody) {
		linkedList *cur = stoneBody;
		while (cur) {
			int *pos = cur->data;
			removeForm(stone, pos[0], pos[1]);
			cur = cur->next;
		}
		freeList(&stoneBody);
	}
	freeForm(stone);
	stoneBody = 0;
	stone = 0;
}

