#include "grass.h"
float grassA[3] = {250.2, 210.2, 85.7};
float grassB[3] = {46.0, 173.0, 49.0};

Form *makeGrass() {
	Form *grass = makePlant();
	grass->id = GRASS;
	setStat(grass, GROWTH, 0.1);
	setStat(grass, PULL, 0.1);
	setStat(grass, BEAT, 5);
	setStat(grass, CYCLE, 5);
	setStat(grass, LIFETIME, 4);

	Nub *ren = growRenderNub(grass);
	RenderObject *rob = ren->data;
	rob->data = grass;
	rob->render = renderGrass;

	return grass;
}

void placeGrass(int x, int y) {
	if (checkGrass(x, y)) {
		return;
	}
	Form *dirt = checkSoil(x, y);
	if (dirt) {// && *getStat(dirt, ECO) > 0.75f) {
		Form *grass = makeGrass();
		placeForm(grass, x, y);
		/*
			 TileSet *ts = getTile(2);
			 editData(ts->color, (int)grass->pos[0], (int)grass->pos[1], 0, 1);
			 addToList(&grassList, grass);// makePos(x, y));
			 */
	}
}


bool growGrass(Form *g) {
	if (!drawing) {
		printf("grwoing grass %p\n" , g);
	}
	float *stage = getStat(g, STAGE);
	(*stage)++;
	if (*stage == 1) {
		/*
			 float *tile = getStat(g, "tile");
			 TileSet *ts = getTile((int)(*tile));
			 editData(ts->color, (int)g->pos[0], (int)g->pos[1], 1, 1);
			 */
		setStat(g, GROWTH, 1);
		setStat(g, BEAT, 1);
		setStat(g, CYCLE, 15);
		//setStat(g, LOSS, 0.001);
		setStat(g, LOSS, 0.01);
		setStat(g, ROOTS, 0.25);
		setStat(g, COVER, evaporation/5);
		calcFlow(g->pos[0], g->pos[1]);
	} else if (*stage == 2) {
		//changeBio(g->pos[0], g->pos[1], 0.25);
		setStat(g, ROOTS, 0.75);
		calcFlow(g->pos[0], g->pos[1]);
		setStat(g, CYCLE, 25);
		setStat(g, GROWTH, 0.6);
		setStat(g, COVER, evaporation/3);
		/*
			 g->id = 3;
			 setStat(g, "tile", 3);
			 */
	} else if (*stage == 3) {
		spreadGrass(g->pos[0], g->pos[1]);
		setStat(g, CYCLE, 35);
	} 
	return true;
}

void spreadGrass(int x, int y) {
	for (int i = 0; i < 8; i++) {
		int *d = getDir8(i);
		placeGrass(x + d[0], y + d[1]);
	}
}

void grassColor(Form *g) {
	float eco = *getStat(g, ECO);
	float growth = *getStat(g, GROWTH);
	// should be some sort of stasis point
	// if the plant has less eco than stasis its dying
	// if more its growing
	float ct = eco;// / 0.5;//growth;
	Nub *sigil = findNub(g, 1);
	if (sigil) {
		Sigil *skin = findNub(g, 1)->data;
		skin->r = lerp(grassA[0], grassB[0], ct);
		skin->g = lerp(grassA[1], grassB[1], ct);
		skin->b = lerp(grassA[2], grassB[2], ct);
	}
}

Form *checkGrass(int x, int y) {
	Cell *c = getCell(x, y);
	if (c) {
		if (!drawing) {
			printf("checking grass at %i, %i\n", x, y);
		}
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i]) {
				if (c->within[i]->id == GRASS) {
					return c->within[i];
				}
			}
		}
	}
	return NULL;
}

void *renderGrass(void *data) {
	Form *grass = data;
	float stage = *getStat(grass, STAGE);
	if (stage == 0) {
		return NULL;
	}
	float eco = *getStat(grass, ECO);
	RenderCommand reco = {
		.screenPos[0] = worldXToScreenX(grass->pos[0]),// + screenX/2 - frameDim[0]/2;
		.screenPos[1] = worldYToScreenY(grass->pos[1]),// + screenY/2 - frameDim[1]/2;
		.r = lerp(grassA[0], grassB[0], eco),
		.g = lerp(grassA[1], grassB[1], eco),
		.b = lerp(grassA[2], grassB[2], eco),
	};
	if (stage == 1) {
		reco.sigil = ':';
	} else if (stage == 2) {
		reco.sigil = '+';
	} else {
		reco.sigil = '#';
	}
	addRenderCommand(reco);

	return NULL;
}
