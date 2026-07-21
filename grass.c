#include "grass.h"
float grassA[3] = {250.2, 210.2, 85.7};
float grassB[3] = {46.0, 173.0, 49.0};

Form *makeGrass() {
	Form *grass = makePlant();
	grass->id = GRASS;
	setStat(grass, GROWTH, 0.1);
	setStat(grass, PULL, 0.1);
	Nub *plantNub = findNub(grass, PLANTNUB);
	Plant *data = plantNub->data;
	data->beat = 1;
	data->cycle = 5;
	data->lifeTime = 4;

	return grass;
}

bool placeGrass(int x, int y) {
	if (!checkGrass(x, y)) {
		Form *dirt = checkSoil(x, y);
		if (dirt) {// && *getStat(dirt, ECO) > 0.75f) {
			Form *grass = makeGrass();
			placeForm(grass, x, y);
			/*
				 TileSet *ts = getTile(2);
				 editData(ts->color, (int)grass->pos[0], (int)grass->pos[1], 0, 1);
				 addToList(&grassList, grass);// makePos(x, y));
				 */
			return true;
		}
	}
	return false;
}


bool growGrass(Form *g) {
	Nub *plantNub = findNub(g, PLANTNUB);
	if (!plantNub) {
		return false;
	}
	Plant *data = plantNub->data;
	data->stage++;
	if (!drawing) {printf("grwoing grass %p stage %i\n" , g, data->stage);}
	if (data->stage == 1) {
		data->cycle = 15;
		Nub *ren = growRenderNub(g);
		RenderObject *rob = ren->data;
		rob->data = g;
		rob->render = renderGrass;
		setStat(g, GROWTH, 0.8);
		//setStat(g, LOSS, 0.001);
		setStat(g, LOSS, 0.02);
		setStat(g, ROOTS, 0.25);
		setStat(g, COVER, evaporation/5);
		calcFlow(g->pos[0], g->pos[1]);
	} else if (data->stage == 2) {
		//changeBio(g->pos[0], g->pos[1], 0.25);
		//setStat(g, CYCLE, 25);
		data->cycle = 25;
		setStat(g, ROOTS, 0.75);
		calcFlow(g->pos[0], g->pos[1]);
		setStat(g, GROWTH, 0.6);
		setStat(g, COVER, evaporation/3);
		/*
			 g->id = 3;
			 setStat(g, "tile", 3);
			 */
	} else if (data->stage == 3) {
		data->cycle = 35;
		spreadGrass(g);
	} else {
		spreadGrass(g);
		addEco(g->pos[0], g->pos[1], *getStat(g, ECO));
	}
	return true;
}

float grassCost = 0.12f;
void spreadGrass(Form *g) {
	float *eco = getStat(g, ECO);
	int spawn = *eco / grassCost;
	for (int i = 0; i < 16; i++) {
		int point = randomInt(8);
		int p[2] = {g->pos[0], g->pos[1]}; 
		int *d = getDir8(point);
		incPos(p, p+1, d[0], d[1]);
		if (placeGrass(p[0], p[1])) {
			*eco -= grassCost;
			if (spawn > 0) {
				spawn--;
			} else {
				break;
			}
		}
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
	Nub *plantNub = findNub(grass, PLANTNUB);
	Plant *plant = plantNub->data;
	if (!plant) {
		return NULL;
	}
	float eco = clampF(*getStat(grass, ECO), 0, 1);
	RenderCommand reco = {
		.screenPos[0] = worldXToScreenX(grass->pos[0]),// + screenX/2 - frameDim[0]/2;
		.screenPos[1] = worldYToScreenY(grass->pos[1]),// + screenY/2 - frameDim[1]/2;
		.r = lerp(grassA[0], grassB[0], eco),
		.g = lerp(grassA[1], grassB[1], eco),
		.b = lerp(grassA[2], grassB[2], eco),
		.layer = GRASSLAYER,
	};
	reco.sigil = grassStamps[plant->stage-1];
	addRenderCommand(reco);

	return NULL;
}
