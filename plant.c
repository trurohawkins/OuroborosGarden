#include "plant.h"
#include "grass.h"
#include "flower.h"

int plantStats = 11;

void initPlants() {
	scheduleEvent(1, plantsAction, 0, plantsInterval);
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
					if (getStat(plant, GROWTH)) {// && plant->id != 2 && plant->id != 3) {
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
	initStats(plant, plantStats);
	addStat(plant, ECO, 0);
	addStat(plant, GROWTH, 0);
	addStat(plant, PULL, 0);
	addStat(plant, LOSS, 0);
	// affects the output of dirt patch
	addStat(plant, ROOTS, 0);
	// affects evaporation in dirt
	addStat(plant, COVER, 0);

	addStat(plant, LIFE, 0);
	addStat(plant, BEAT, 0);

	// at what interval in its lifeTime it grows
	addStat(plant, CYCLE, 0);
	addStat(plant, STAGE, 0);
	//lifeTime, number of stages before it dies
	addStat(plant, LIFETIME, 1);

	return plant;
}

bool lifeCycle(Form *plant) {
	if (!plant) {
		return false;
	}
	float *life = getStat(plant, LIFE);
	float beat = *getStat(plant, BEAT);
	(*life)++;
	if ((int)*life % (int)beat == 0) {
		if (!drawing) {printf("plant beat %p\n", plant);}
		// plant loses some eco
		float *eco = getStat(plant, ECO);
		float loss = *getStat(plant, LOSS); 
		float lifeTime = *getStat(plant, LIFETIME);
		float stage = *getStat(plant, STAGE);
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
		//gathering references to soil around plant
		Form *ground[9];
		ground[0] = checkSoil(x, y);
		for (int j = 0; j < 8; j++) {
			int *dir = getDir8(j);
			ground[j+1] = checkSoil(x + dir[0], y + dir[1]);
		}
		float pull = *getStat(plant, PULL);
		float *growth = getStat(plant, GROWTH);
		//pulling eco from the surrounding ground
		if (*eco < *growth) {
			pull = min(pull, *growth - *eco);
			float gather = 0;
			for (int j = 0; j < 9; j++) {
				if (ground[j]) {
					float *dirtEco = getStat(ground[j], ECO);
					if (dirtEco) {
						if (*dirtEco > pull) {
							gather = pull;
							changeEco(ground[j], -pull);
						} else {
							gather += *dirtEco;
							changeEco(ground[j], -(*dirtEco));
						}
						if (gather >= pull) {
							break;
						}
					}
				}
			}
			if (gather > 0) {
				if (!drawing) {printf("   eco gather %f\n", gather);}
				*eco += gather;
			}
		}	
		if (!drawing) {printf("     final eco: %f\n", *eco);}
		// if they gather enough eco they grow
		// if they are old enough
		if (*eco >= *growth && *life >= *getStat(plant, CYCLE)) {
			if (grow(plant)) {
				*life = 0;
			}
		}

		plantEco(plant);
	}
	return true;
}

bool grow(Form *plant) {
	if (plant->id == GRASS) {
		growGrass(plant);
	} else if (plant->id == FLOWER) {
		growFlower(plant);
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
