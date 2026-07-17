#include "flower.h"

float flowerGrowth = 0.5;
float fullFower = 3;

Form *makeFlower() {
	Form *f = makePlant();
	f->id = 4;
	setStat(f, GROWTH, 0.5);
	setStat(f, PULL, 0.2);
	setStat(f, BEAT, 2);
	setStat(f, CYCLE, 2);
	setStat(f, LIFETIME, 4);
	return f;
}

bool placeFlower(int x, int y) {
	if (!checkFormID(x, y, FLOWER)) {
		Form *dirt = checkSoil(x, y);
		if (dirt) {
			Form *flower = makeFlower();
			placeForm(flower, x, y);
			return true;
		}
	}
	return false;
}

bool growFlower(Form *f) {
	float *stage = getStat(f, STAGE);
	if (*stage == 0) {
		int x = f->pos[0];
		int y = f->pos[1];
		Form *dirt = checkSoil(x, y);
		/*
		if (dirt && *getStat(dirt, "bio") < 0.5f) {
			return false;
		}
		if (checkStat(f->pos[0], f->pos[1], "block")) {
			//printf("no grow under something\n");
			return false;
		}
		*/
		if (randPercent() < 0.95) {
			//return false;
		}
	}	
	(*stage)++;
	if (*stage == 1) {
		Nub *ren = growRenderNub(f);
		RenderObject *rob = ren->data;
		rob->data = f;
		rob->render = renderFlower;
		setStat(f, GROWTH, 1);
		setStat(f, PULL, 0.5);
		setStat(f, LOSS, 0.1);
		//setStat(f, BEAT, 30);
		setStat(f, CYCLE, 3);
		setStat(f, ROOTS, 0.5);
		calcFlow(f->pos[0], f->pos[1]);
	} else if (*stage < 5) {
		//Anim *anim = ((Anim**)f->anim)[0];
		//changeSprite(anim, *stage - 1);
		if (*stage == 2) {
			setStat(f, ROOTS, 1);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, COVER, 0.005);
			setStat(f, LOSS, 0.05);
		} else if (*stage == 3) {
			//changeBio(f->pos[0], f->pos[1], -0.5);
			setStat(f, COVER, 0.01);
			setStat(f, CYCLE, 9);
		} else if (*stage == 4) {
			setStat(f, ROOTS, 0);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, CYCLE, 12);
			//setStat(f, LOSS, 0);
			setStat(f, LOSS, 0.01);
			spreadFlower(f->pos[0], f->pos[1]);
		}
	} else {
		//changeBio(f->pos[0], f->pos[1], 0.05);
		addEco(f->pos[0], f->pos[1], *getStat(f, ECO));
	}
	return true;
}

void *renderFlower(void *data) {
	Form *flower = data;

	float *stage = getStat(flower, STAGE);
	RenderCommand reco = {
		.screenPos[0] = worldXToScreenX(flower->pos[0]),
		.screenPos[1] = worldYToScreenY(flower->pos[1]),
		.r = 255, 
		.g = 0,
		.b = 255,
		.layer = FLOWERLAYER,
	};
	if (*stage == 1) {
		reco.sigil = flowerStamps[0];
	} else if (*stage == 2) {
		reco.sigil = flowerStamps[1];
	} else if (*stage == 3) {
		reco.sigil = flowerStamps[2];
	} else if (*stage == 4) {
		reco.sigil = flowerStamps[3];
	} else if (*stage == 5) {
		reco.sigil = flowerStamps[4];
	} else if (*stage == 6) {
		reco.sigil = flowerStamps[5];
	} else {
		reco.sigil = flowerStamps[6];
	}
	addRenderCommand(reco);
	return NULL;
}

void spreadFlower(int x, int y) {
	int start = randomInt(8);
	for (int i = 0; i < 8; i++) {
		int *d = getDir8((start+i)%8);
		if (placeFlower(x + d[0], y + d[1])) {
			return;
		}
	}
}
