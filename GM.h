#pragma once
#include "garden.h"

typedef struct {
	Menu *curMenu;
	Menu *startMenu;
	Menu *pauseMenu;

	int level;
	bool won;

	int curPlants;
	int plantGoal;
} GameMaster;

extern GameMaster GM;

void makeMenus();
void freeMenus();
void plantCount(int amnt);
void checkPlants();
void endLevel();

void pressUp(void *guy, float val);
void pressLeft(void *guy, float val);
void pressDown(void *guy, float val);
void pressRight(void *guy, float val);
void pressSpace(void *, float val);

void pauseGame(void *, float val);
void resumeGame();
void restartGame();
void returnToMenu();

