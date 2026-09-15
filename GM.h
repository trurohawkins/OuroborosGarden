#pragma once
#include "garden.h"

typedef struct {
	Menu *curMenu;
	Menu *startMenu;
	Menu *pauseMenu;
	Menu *winScreen;
	Editor *editor;

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
void loadNewLevel();
void renderGM();

void pressUp(void *guy, float val);
void pressLeft(void *guy, float val);
void pressDown(void *guy, float val);
void pressRight(void *guy, float val);
void pressSpace(void *, float val);

void pauseGame(void *, float val);
void resumeGame();
void restartGame();
void returnToMenu();

void pressEditorToggle(void *gm, float val);
void pressEditorMake(void *gm, float val);
void pressEditorRecipe(void *gm, float val);
void pressEditorInspect(void *gm, float val);
void pressEditorRemove(void *gm, float val);
void pressEditorSave(void *gm, float val);


