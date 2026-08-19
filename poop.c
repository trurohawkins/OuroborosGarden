#include "poop.h"

int poopLifeTime = 50;
int pooStamp = -1;

Form *makePoop() {
	Form *poo = makeForm(POOP);
	
	Nub *ren = growRenderNub(poo);
	RenderObject *rob = ren->data;
	rob->data = poo;
	rob->render = renderPoop;

	if (pooStamp == -1) {
		pooStamp = createStamp("&");
	}

	Actor *actor = makeFormActor(poo);
	Action *action = makeAction(0, poopAction, poo);
	addAction(actor, action);
	addActor(actor);

	initStats(poo, 2);
	//decay
	addStat(poo, 0, 10);
	//counter
	addStat(poo, 1, 0);

	return poo;
}

int poopAction(void *data, Action *a, float delta) {
	Form *f = data;
	float *decay = getStat(f, 0);
	float *counter = getStat(f, 1);
	if (*counter < poopLifeTime) {
		(*counter)++;
	} else {
		if (*decay > 0) {
			(*decay)--;
			pushEco(f->pos[0], f->pos[1], 0.25);
			(*counter) = 0;
		} else {
			int x = f->pos[0];
			int y = f->pos[1];
			removeForm(f, x, y);
			Actor *a = findNub(f, 2)->data;
			a->deleteMe = true;
			freeForm(f);
			placeFlower(x, y);
		}
	}
}

void *renderPoop(void *data) {
	Form *poo = data;
	RenderCommand reco = {
		.type = 0,
		.index = pooStamp,
		.layer = POOPLAYER,
	};
	PosColor pc = {
		.pos = {
			.x = worldXToScreenX(poo->pos[0]),
			.y = worldYToScreenY(poo->pos[1]),
		},
		.color = {
			.vals = {
				120, 20, 60
			},
		},
	};
	memcpy(reco.data, &pc, sizeof(PosColor));
	addRenderCommand(reco);
}
