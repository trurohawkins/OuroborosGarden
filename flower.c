#include "flower.h"

float flowerGrowth = 0.5;
float fullFower = 3;

//float sprout[3] = {23, 164, 239};
//float sDying[3] = {23, 239, 194};
float sprout[3] = {255, 114, 23};
float sDying[3] = {242, 173, 0};
float fruit[3] = {188, 23, 239};
//float fDying[3] = {239, 23, 103};
float fDying[3] = {255, 0, 100};

Form *makeFlower() {
	Form *f = makePlant();
	f->id = FLOWER;
	setStat(f, GROWTH, 1);
	setStat(f, PULL, 0.1);
	setStat(f, LOSS, 0.02);
	Nub *plant = findNub(f, PLANTNUB);
	Plant *data = plant->data;
	data->beat = 1;
	data->cycle = 8;
	data->lifeTime = 4;
	return f;
}

Form *placeFlower(int x, int y) {
	if (!checkFormID(x, y, FLOWER)) {
		Form *dirt = checkSoil(x, y);
		if (checkFormID(x, y, DIRT)) {
			Form *flower = makeFlower();
			if (placeForm(flower, x, y)) {
				return flower;
			} else {
				freeForm(flower);
			}
		}
	}
	return NULL;
}

bool growFlower(Form *f) {
	Nub *plantNub = findNub(f, PLANTNUB);
	if (!plantNub) {
		return false;
	}
	Plant *data = plantNub->data;
	if (data->stage == 0) {
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
	data->stage++;
	if (data->stage == 1) {
		Nub *ren = growRenderNub(f);
		RenderObject *rob = ren->data;
		rob->data = f;
		rob->render = renderFlower;
		setStat(f, GROWTH, 1);
		//setStat(f, PULL, 0.5);
		setStat(f, LOSS, 0.1);
		//setStat(f, BEAT, 30);
		setStat(f, ROOTS, 0.5);
		calcFlow(f->pos[0], f->pos[1]);
		plantCount(1);
	} else if (data->stage < 5) {
		//Anim *anim = ((Anim**)f->anim)[0];
		//changeSprite(anim, *stage - 1);
		if (data->stage == 2) {
			setStat(f, ROOTS, 1);
			calcFlow(f->pos[0], f->pos[1]);
			setStat(f, COVER, 0.005);
			setStat(f, LOSS, 0.05);
		} else if (data->stage == 3) {
			//changeBio(f->pos[0], f->pos[1], -0.5);
			setStat(f, COVER, 0.01);
		} else if (data->stage == 4) {
			data->cycle = 16;
			setStat(f, ROOTS, 0);
			calcFlow(f->pos[0], f->pos[1]);
			//setStat(f, LOSS, 0);
			setStat(f, LOSS, 0.01);
			spreadFlower(f->pos[0], f->pos[1]);
		}
	} else {
		//changeBio(f->pos[0], f->pos[1], 0.05);
		spreadFlower(f->pos[0], f->pos[1]);
		addEco(f->pos[0], f->pos[1], *getStat(f, ECO));
	}
	return true;
}

void *renderFlower(void *data) {
	Form *flower = data;

	Nub *plantNub = findNub(flower, PLANTNUB);
	Plant *plant = plantNub->data;
	if (!plant) {
		return NULL;
	}
	float eco = clampF(*getStat(flower, ECO), 0, 1);
	RenderCommand reco = {
		.layer = FLOWERLAYER,
		.type = 0,
		.index = flowerStamps[plant->stage-1],
	};
	PosColor pc = {
		.pos = {
		.x = worldXToScreenX(flower->pos[0]),
		.y = worldYToScreenY(flower->pos[1]),
		}
	};
	if (plant->stage <= 2) {
		pc.color.vals[0] = lerp(sDying[0], sprout[0], eco);
		pc.color.vals[1] = lerp(sDying[1], sprout[1], eco);
		pc.color.vals[2] = lerp(sDying[2], sprout[2], eco);
	} else {
		pc.color.vals[0] = lerp(fDying[0], fruit[0], eco);
		pc.color.vals[1] = lerp(fDying[1], fruit[1], eco);
		pc.color.vals[2] = lerp(fDying[2], fruit[2], eco);
	}
	memcpy(reco.data, &pc, sizeof(PosColor));
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
