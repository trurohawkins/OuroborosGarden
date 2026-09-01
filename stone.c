#include "stone.h"

Form *stone = 0;
linkedList *stoneBody = 0;

void initStone() {
	if (!stone) {
		stone = makeForm(STONE);
		initStats(stone, 2);
		addStat(stone, COVER, 0.05f);
		addStat(stone, BLOCK, 1);
		Nub *ren = growRenderNub(stone);
		RenderObject *rob = ren->data;
		rob->data = stone;
		rob->render= renderStone;
	}
}

Form *placeStone(int x, int y) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		int *pos = calloc(2, sizeof(int));
		pos[0] = x;//(x % w->x + w->x) % w->x;
		pos[1] = y;//(y % w->y + w->y) % w->y;
		addToList(&stoneBody, pos);
		placeForm(stone, pos[0], pos[1]);
		return stone;
	}
	return NULL;
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
		reco.pos.x = worldXToScreenX(pos[0]);
		reco.pos.y = worldYToScreenY(pos[1]);
		Color color = {
			.rgb = {
				60, 80, 70
			},
		};
		memcpy(reco.data, &color, sizeof(Color));
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

