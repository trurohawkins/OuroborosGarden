#include "grass.h"
float grassA[3] = {165, 153, 7};
float grassB[3] = {24, 101, 2};

Form *makeGrass() {
	Form *grass = makePlant();
	grass->id = GRASS;
	setStat(grass, GROWTH, 0.1);
	setStat(grass, PULL, 0.1);
	Nub *plantNub = findNub(grass, PLANTNUB);
	Plant *data = plantNub->data;
	data->beat = 1;
	data->cycle = 4;
	data->lifeTime = 4;

	return grass;
}

bool placeGrass(int x, int y) {
	if (!checkGrass(x, y)) {
		Form *dirt = checkSoil(x, y);
		if (dirt) {
			Form *grass = makeGrass();
			if (placeForm(grass, x, y)) {
				return true;
			} else {
				freeForm(grass);
			}
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
		setStat(g, LOSS, 0.01);
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

Color grassColor(Form *g) {
	float eco = *getStat(g, ECO);
	float growth = *getStat(g, GROWTH);
	// should be some sort of stasis point
	// if the plant has less eco than stasis its dying
	// if more its growing
	Color c = {
		.rgb = {
			lerp(grassA[0], grassB[0], eco),
			lerp(grassA[1], grassB[1], eco),
			lerp(grassA[2], grassB[2], eco),
		},
	};
}

int grassStamp(Form *grass) {
	Nub *plantNub = findNub(grass, PLANTNUB);
	Plant *plant = plantNub->data;
	if (!plant) {
		return -1;
	}
	return plant->stage - 1;
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
		.layer = GRASSLAYER,
		.type = 0,
		.index = grassStamps[plant->stage-1],
		.pos = {
			.x = worldXToScreenX(grass->pos[0]),// + screenX/2 - frameDim[0]/2;
			.y = worldYToScreenY(grass->pos[1]),// + screenY/2 - frameDim[1]/2;
		},
	};
	Color color = {
		.rgb = {
			lerp(grassA[0], grassB[0], eco),
			lerp(grassA[1], grassB[1], eco),
			lerp(grassA[2], grassB[2], eco),
		},
	};
	if (reco.index > -1) {
		bool figure = false;
		Form *dirt = 0;
		World *w = getWorld();
		int wi = grass->pos[1] * w->x + grass->pos[0];
		Cell c = theWorld.map[wi];
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c.within[i]) {
				int id = c.within[i]->id;
				if (id == SNAKE || id == FLOWER || id == POOP) {
					figure = true;
				} else if (id == DIRT) {
					dirt = c.within[i];
				}
			}
		}
		if (figure && dirt) {
			reco.index = -1;
			Color d = dirtColor(dirt);
			for (int i = 0; i < 3; i++) {
				color.rgb[i] = lerp(color.rgb[i], d.rgb[i], 0.5);
			}
		}
	}
	memcpy(reco.data, &color, sizeof(Color));

	addRenderCommand(reco);

	return NULL;
}
