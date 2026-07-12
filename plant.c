#include "plant.h"

void initPlants() {
	scheduleEvent(1, plantsAction, 0, plantsInterval);
}

void plantsAction(void *) {
	World *w = getWorld();
	/*
		 grassCounter += delta;
		 float frequency = 1.0  / grassBeat;
		 int compound = grassCounter / grassBeat;
		 for (int i = 0; i < compound; i++) {
		 if (grassCounter >= frequency) {
		 grassCounter -= frequency;
	//playAudio(grassSound);
	checkAndDelete(&grassList, checkPlantLife, plantDie);
	}
	}
	*/
	linkedList *dead = 0;
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y*w->x)+x];
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				if (c->within[i]) {
					Form *plant = c->within[i];
					if (getStat(plant, GROWTH) && plant->id != 2 && plant->id != 3) {
						float *counter = getStat(plant, COUNTER);
						float *life = getStat(plant, LIFE);
						if (*counter > *life) {
							if (!lifeCycle(plant)) {
								addToList(&dead, plant);
							}
							*counter = 0;
						} else {
							*counter++;
						}
					}
				}
			}
			deleteList(&dead, plantDie);
		}
	}
}

Form *makePlant() {
	Form *plant = makeForm(PLANT);//0.2, 0.7, 0.5, 1, 1);
	addStat(plant, ECO, 0);
	addStat(plant, GROWTH, 0);
	addStat(plant, PULL, 0);
	addStat(plant, LOSS, 0);
	// affects the output of dirt patch
	addStat(plant, ROOTS, 0);
	// affects evaporation in dirt
	addStat(plant, COVER, 0);

	addStat(plant, LIFE, 0);
	addStat(plant, COUNTER, 0);

	// at what interval in its lifeTime it grows
	addStat(plant, CYCLE, 0);
	addStat(plant, STAGE, 0);
	//lifeTime, number of stages before it dies
	addStat(plant, LIFETIME, 1);

	return plant;
}

bool lifeCycle(Form *plant) {
	if (!plant) {
		printf("no plant\n");
		return false;
	}
	float *life = getStat(plant, LIFE);
	float *eco = getStat(plant, ECO);

	//(*life)++;
	//if ((int)*life % (int)beat == 0) {
	float loss = *getStat(plant, LOSS); 
	float lifeTime = *getStat(plant, LIFETIME);
	float stage = *getStat(plant, STAGE);
	if (stage < lifeTime && *eco - loss >= 0) {
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
			*eco += gather;
			//printf("gathered %f -> %f\n", gather, *eco);
		}
	}
	//printf("eco: %f, growth: %f -- life %f >= %f\n", *eco, *growth, *life, *getStat(plant, "cycle"));
	if (*eco >= *growth && *life >= *getStat(plant, CYCLE)) {
		if (grow(plant)) {
			*life = 0;
		}
	}

	plantEco(plant);
	return true;
}

bool grow(Form *plant) {

}

void plantDie(void *plant) {
	Form *p = plant;
	removeForm(plant, p->pos[0], p->pos[1]);
	freeForm(plant);
}

void plantEco(Form *plant) {
	if (plant->id == 2 || plant->id == 3) {
		//grassColor(plant);
	}
}

