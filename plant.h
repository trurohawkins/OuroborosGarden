#pragma once

void initPlants();
void plantsAction(void *);
Form *makePlant();
bool lifeCycle(Form *plant);
bool grow(Form *plant);
void plantDie(void *plant);
void plantEco(Form *plant);

