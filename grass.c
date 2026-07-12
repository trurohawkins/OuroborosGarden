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


	return grass;
}

void placeGrass(int x, int y) {
	if (checkGrass(x, y)) {
		return;
	}
	Form *dirt = checkSoil(x, y);
	if (dirt && *getStat(dirt, ECO) > 0.75f) {
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
		Sigil *skin = createSigil(g)->data;
		skin->symbol = '#';
		skin->figure = true;
		skin->r = grassB[0];
		skin->g = grassB[1];
		skin->b = grassB[2];
		grassColor(g);
		setStat(g, GROWTH, 1);
		setStat(g, BEAT, 1);
		setStat(g, CYCLE, 15);
		setStat(g, LOSS, 0.001);
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
		Sigil *skin = findNub(g, 1)->data;
		skin->figure = false;
		/*
			 g->id = 3;
			 setStat(g, "tile", 3);
			 */
		grassColor(g);
	} else if (*stage == 3) {
		spreadGrass(g->pos[0], g->pos[1]);
		setStat(g, CYCLE, 35);
		grassColor(g);
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
	Sigil *skin = findNub(g, 1)->data;
	if (skin) {
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

