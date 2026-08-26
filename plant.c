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
	grassStamps[0] = createStamp("\u2591", "\u2591");//("\u2261");
	grassStamps[1] = createStamp("\u2592", "\u2592");
	grassStamps[2] = createStamp("\u2593", "\u2593");
	grassStamps[3] = -1;
	flowerStamps[0] = createStamp("\u26BA", 0);
	flowerStamps[1] = createStamp("\u2021", 0);
	flowerStamps[2] = createStamp("\u2BDE", 0);//#","P");
	flowerStamps[3] = createStamp("\u2B53", 0);
	flowerStamps[4] = createStamp(0, "\u26BA");
	flowerStamps[5] = createStamp(0, "\u2021");
	flowerStamps[6] = createStamp(0, "\u2BDE");//#","P");
	flowerStamps[7] = createStamp(0, "\u2B53");
	//flowerStamps[1] = createStamp("\u26B6",0);
	//flowerStamps[2] = createStamp("\u03D8", 0);//#","P");
	//flowerStamps[2] = createStamp("\u26B5", 0);//#","P");
	//flowerStamps[3] = createStamp("\u26B9", 0);
	//flowerStamps[3] = createStamp("\u047D", 0);
	/*
	flowerStamps[0] = createStamp("\U0001683D", 0);
	flowerStamps[1] = createStamp("\U00016912", 0);
	flowerStamps[2] = createStamp("\uAA51", 0);
	flowerStamps[3] = createStamp("\U0001F73E", 0);
	*/
	//flowerStamps[3] = createStamp("\U000168E6", 0);
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

void plantsAction(void *) {
	World *w = getWorld();
	linkedList *dead = 0;
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y*w->x)+x];
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				if (c->within[i]) {
					Form *plant = c->within[i];
					if (findNub(plant, PLANTNUB)) {// && plant->id != 2 && plant->id != 3) 
						if (!lifeCycle(plant)) {
							plantDie(plant);
						}
					}
				}
			}
		}
	}
	checkPlants();
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
		//if (stage < lifeTime && *eco - loss >= 0) 
		if (!drawing) {printf("   eco loss %f\n", loss);}
		if (*eco - loss >= 0) {
			*eco -= loss;
		} else {
			// seeds dont die but do lose water
			if (data->stage > 0) {
				return false;
			} else {
				*eco = 0;
			}
		}
		int x = plant->pos[0];
		int y = plant->pos[1];
		Form *soil = checkSoil(x, y);
		float *soilEco = getStat(soil, ECO);
		if ((soilEco && equal(*soilEco, 0)) || !soilEco) {
			if (!drawing) {printf("	soilEco %p: %f\n", soilEco, *soilEco);}
			//return true;
		}
		float pull = *getStat(plant, PULL);
		float *growth = getStat(plant, GROWTH);
		if (!drawing) {printf("     pull: %f\n", pull);}
		//pulling eco from the surrounding ground
		pull = min(pull, *growth - *eco);
		if (!drawing) {printf("     pull == %f\n", pull);}
		float gather = pullEco(x, y, pull);
		if (gather > 0) {
			if (!drawing) {printf("   eco gather %f\n", gather);}
			*eco = clampF(*eco + gather, 0, 1);
		}
		if (!drawing) {printf("     final eco: %f\n", *eco);}
		// if they gather enough eco they grow
		// if they are old enough
		if (!drawing) {printf("    growth: %f. life: %d >= cycle %d\n", *growth, data->life, data->cycle);}
		if (*eco >= *growth && data->life >= data->cycle) {
			if (!checkStat(x, y, BLOCK)) {
				if (!drawing) {printf(    "GROW\n");}
				if (grow(plant)) {
					data->life = 0;
					if (data->stage == 1) {
						plantCount(1);
					}
				} else {
					return false;
				}
				if  (data->stage > data->lifeTime) {
					return false;
				}
			}
		}
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
	plantCount(-1);
}

#include "grass.c"
#include "flower.c"
