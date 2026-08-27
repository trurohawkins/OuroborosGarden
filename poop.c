#include "poop.h"

int poopLifeTime = 40;
int pooStamp[2] = {-1, -1};

Form *makePoop() {
	Form *poo = makeForm(POOP);
	
	Nub *ren = growRenderNub(poo);
	RenderObject *rob = ren->data;
	rob->data = poo;
	rob->render = renderPoop;

	if (pooStamp[0] == -1) {
		//pooStamp = createStamp("\U0001694B", 0);
		//pooStamp = createStamp("\u2C5E", 0);
		//snowman
		//pooStamp = createStamp("\u26C7", 0);
		//pooStamp = createStamp("\u28F0", "\u28E7");
		//char *stamp = "\u07F7";
		char *stamp = "\u25FC";
		pooStamp[0] = createStamp(0, stamp);
		pooStamp[1] = createStamp(stamp, 0);
	}

	Actor *actor = makeFormActor(poo);
	Action *action = makeAction(0, poopAction, poo);
	addAction(actor, action);
	addActor(actor);

	initStats(poo, 4);
	//decay
	addStat(poo, 0, 20);
	//counter
	addStat(poo, 1, 0);
	addStat(poo, 2, randomInt(2));
	addStat(poo, 3, -1);//randomInt(14));

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
			addEco(f->pos[0], f->pos[1], 1.0);
			(*counter) = 0;
			float *color = getStat(f, 3);
			if (*color < 0) {
				*color = randomInt(14);
			} else {
				*color = (int)(*color + 1) % 14;
			}
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
	int stamp = *getStat(poo, 2);
	RenderCommand reco = {
		.type = 0,
		.index = pooStamp[stamp],
		.layer = POOPLAYER,
	};
	float r = 255;
	float g = 255;
	float b = 255;
	int color = *getStat(poo, 3) * 3;
	if (color >= 0) {
		r = rainbowColors[color];
		g = rainbowColors[color+1];
		b = rainbowColors[color+2];
	}
	PosColor pc = {
		.pos = {
			.x = worldXToScreenX(poo->pos[0]),
			.y = worldYToScreenY(poo->pos[1]),
		},
		.color = {
			.vals = {
				//13, 194, 79
				//255, 255, 255
				r, g, b	
			},
		},
	};
	memcpy(reco.data, &pc, sizeof(PosColor));
	addRenderCommand(reco);
}
