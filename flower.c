#include "flower.h"

float flowerGrowth = 0.5;
float fullFower = 3;

Form *makeFlower() {
	Form *f = makePlant();
	f->id = 4;
	setStat(f, GROWTH, 0.2);
	setStat(f, PULL, 0.2);
	setStat(f, BEAT, 2);
	setStat(f, CYCLE, 2);
	setStat(f, LIFETIME, 9);
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
		setStat(f, LOSS, 0.01);
		//setStat(f, BEAT, 30);
		setStat(f, CYCLE, 3);
	} else if (*stage < 8) {
		//Anim *anim = ((Anim**)f->anim)[0];
		//changeSprite(anim, *stage - 1);
		if (*stage == 2) {
			setStat(f, ROOTS, 0.25);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, COVER, 0.005);
			setStat(f, LOSS, 0.05);
		} else if (*stage == 3) {
			setStat(f, ROOTS, 0.5);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, COVER, 0.007);
		} else if (*stage == 4) {
			//changeBio(f->pos[0], f->pos[1], -0.5);
			setStat(f, ROOTS, 0.75);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, COVER, 0.01);
			setStat(f, CYCLE, 9);
			setStat(f, GROWTH, 1.2);
		} else if (*stage == 5) {
			//changeBio(f->pos[0], f->pos[1], -0.5);
			setStat(f, ROOTS, 1);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, GROWTH, 0.5);
		} else if (*stage == 6) {
			setStat(f, ROOTS, 0);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, CYCLE, 12);
			setStat(f, PULL, 0);
			//setStat(f, LOSS, 0);
			setStat(f, LOSS, 0.01);
		}
	} else {
		//changeBio(f->pos[0], f->pos[1], 0.05);
		addEco(f->pos[0], f->pos[1], 1);
	}
	return true;
}

void *renderFlower(void *data) {
	Form *flower = data;

	float *stage = getStat(flower, STAGE);
	RenderCommand reco = {
		.screenPos[0] = worldXToScreenX(flower->pos[0]),
		.screenPos[1] = worldYToScreenY(flower->pos[1]),
		.r = 200,
		.g = 75,
		.b = 180,
	};
	if (*stage == 1) {
		reco.sigil = ',';
	} else if (*stage == 2) {
		reco.sigil = '.';
	} else if (*stage == 3) {
		reco.sigil = ';';
	} else if (*stage == 4) {
		reco.sigil = ':';
	} else if (*stage == 5) {
		reco.sigil = 'i';
	} else if (*stage == 6) {
		reco.sigil = 'I';
	} else {
		reco.sigil = '_';
	}
	addRenderCommand(reco);
	return NULL;
}
