#pragma once

typedef struct {
	int life;//counter
	int beat;//how often we do growing operations
	int cycle;//how much before we grow to next stage

	int stage;
	int lifeTime;// how many stages until death
} Plant;


void initPlants();
Plant *getPlant(Form *p);
void plantsAction(void *);
Form *makePlant();
bool lifeCycle(Form *plant);
bool grow(Form *plant);
void plantDie(void *plant);
void plantEco(Form *plant);

