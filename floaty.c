#include "floaty.h"

int floatStamps[2] = {-1, -1};
float floatColors[3] = {
	155, 255, 196
};
Form *makeFloaty(int type) {
	Form *flo = makeForm(FLOATY);
	Nub *ren = growRenderNub(flo);
	RenderObject *rob = ren->data;
	rob->data = flo;
	rob->render = renderFloaty;

	if (floatStamps[0] == -1) {
		floatStamps[0] = createStamp("\U0001682A", 0);
		floatStamps[1] = createStamp(0, "\U0001682A");
	}

	Actor *actor = makeFormActor(flo);
	Action *action = makeAction(0, floatyAction, flo);
	addAction(actor, action);
	addActor(actor);

	initStats(flo, 3);
	//counter
	addStat(flo, 0, 0);
	//speed
	addStat(flo, 1, 20);
	//lifeTime
	addStat(flo, 2, 5);
	return flo;
}

int floatyAction(void *data, Action *a, float delta) {
	Form *f = data;
	float *counter = getStat(f, 0);
	float *interval = getStat(f, 1);
	float *lifeTime = getStat(f, 2);
	if (*counter >= *interval) {
		(*counter) = 0;
		if (*lifeTime > 0) {
			(*lifeTime)--;
			removeForm(f, f->pos[0], f->pos[1]);
			int *d = getDir4(0);
			incPos(f->pos, f->pos+1, d[0], d[1]);
			placeForm(f, f->pos[0], f->pos[1]);
		} else {
			removeForm(f, f->pos[0], f->pos[1]);
			Actor *a = findNub(f, 2)->data;
			a->deleteMe = true;
			freeForm(f);
		}
	} else {
		(*counter)++;
	}
}

void *renderFloaty(void *data) {
	Form *flo = data;
	int counter = *getStat(flo, 0);
	RenderCommand reco = {
		.type = 0,
		.index = floatStamps[0 + (counter%2)],
		.layer = UILAYER,
	};
	PosColor pc = {
		.pos = {
			.x = worldXToScreenX(flo->pos[0]),
			.y = worldYToScreenY(flo->pos[1]),
		},
		.color = {
			.vals = {
				floatColors[0], floatColors[1], floatColors[2]
			},
		},
	};
	memcpy(reco.data, &pc, sizeof(PosColor));
	addRenderCommand(reco);
}

