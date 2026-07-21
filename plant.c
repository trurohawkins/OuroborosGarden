#include "plant.h"
#include "grass.h"
#include "flower.h"

int plantStats = 11;
int grassStamps[4];
int flowerStamps[8];

void initPlants() {
	addTimedEvent(plantsAction, 0, plantsInterval);
	addTimedEvent(dirtFlow, 0, spreadInterval);
	addTimedEvent(ecoEvaporation, 0, evapInterval);
	grassStamps[0] = createStamp("\u2591");//("\u2261");
	grassStamps[1] = createStamp("\u2592");
	grassStamps[2] = createStamp("\u2593");
	grassStamps[3] = -1;
	flowerStamps[0] = createStamp("o");
	flowerStamps[1] = createStamp("0");
	flowerStamps[2] = createStamp("$");
	flowerStamps[3] = createStamp("!");
	flowerStamps[4] = createStamp("L");
	flowerStamps[5] = createStamp("$");
	flowerStamps[6] = createStamp("$");
}

void plantsAction(void *) {
	World *w = getWorld();
	linkedList *dead = 0;
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y*w->x)+x];
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				if (c->within[i]) {
					Form *plant = c->within[i];
					if (findNub(plant, PLANTNUB)) {// && plant->id != 2 && plant->id != 3) {
						if (!lifeCycle(plant)) {
							//addToList(&dead, plant);
							plantDie(plant);
						}
					}
				}
			}
			//deleteList(&dead, plantDie);
		}
	}
}

Form *makePlant() {
	Form *plant = makeForm(PLANT);//0.2, 0.7, 0.5, 1, 1);
	Nub *plantNub = growNub(plant);
	plantNub->type = PLANTNUB;
	plantNub->owned = true;
	plantNub->data = calloc(1, sizeof(Plant));
	initStats(plant, plantStats);
	addStat(plant, ECO, 0);
	addStat(plant, GROWTH, 0);
	addStat(plant, PULL, 0);
	addStat(plant, LOSS, 0);
	// affects the output of dirt patch
	addStat(plant, ROOTS, 0);
	// affects evaporation in dirt
	addStat(plant, COVER, 0);
	return plant;
}

Plant *getPlant(Form *p) {
	if (!p) {
		return 0;
	}
	Nub *plantNub = findNub(p, PLANTNUB);
	if (!plantNub) {
		return 0;
	}
	return plantNub->data;
}

bool lifeCycle(Form *plant) {
	Plant *data = getPlant(plant);
	if (!data) {
		return true;
	}
	data->life++;
	if (data->life % data->beat == 0) {
		// plant loses some eco
		float *eco = getStat(plant, ECO);
		if (!drawing) {printf("plant beat %p\n", plant);}
		if (!drawing) {printf("    starting eco %f\n", *eco);}
		float loss = *getStat(plant, LOSS); 
		//if they lose too much they die
		//if (stage < lifeTime && *eco - loss >= 0) {
		if (*eco - loss >= 0) {
			if (!drawing) {printf("   eco loss %f\n", loss);}
			*eco -= loss;
		} else {
			return false;
		}
		int x = plant->pos[0];
		int y = plant->pos[1];
		float pull = *getStat(plant, PULL);
		float *growth = getStat(plant, GROWTH);
		if (!drawing) {printf("     pull: %f\n", pull);}
		//pulling eco from the surrounding ground
		if (*eco < *growth) {
			pull = min(pull, *growth - *eco);
			float gather = pullEco(x, y, pull);
			if (gather > 0) {
				if (!drawing) {printf("   eco gather %f\n", gather);}
				*eco += gather;
			}
		}	else {
			if (!drawing) {printf("    no eco gathered %f < %f\n", *eco, *growth);}
		}
		if (!drawing) {printf("     final eco: %f\n", *eco);}
		// if they gather enough eco they grow
		// if they are old enough
		if (!drawing) {printf("    growth: %f. life: %d >= cycle %d\n", *growth, data->life, data->cycle);}
		if (*eco >= *growth && data->life >= data->cycle) {
			if (grow(plant)) {
				data->life = 0;
			} else {
				return false;
			}
			if  (data->stage > data->lifeTime) {
				return false;
			}
		}

		plantEco(plant);
	}
	return true;
}

bool grow(Form *plant) {
	if (plant->id == GRASS) {
		return growGrass(plant);
	} else if (plant->id == FLOWER) {
		return growFlower(plant);
	}
}

void plantDie(void *plant) {
	Form *p = plant;
	removeForm(plant, p->pos[0], p->pos[1]);
	freeForm(plant);
}

void plantEco(Form *plant) {
	if (plant->id == GRASS) {
		//grassColor(plant);
	}
}

#include "grass.c"
#include "flower.c"
