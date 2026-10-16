#include "garden.h"

void makeGarden() {
	GM.curMenu = NULL;
	//makeWorld(worldX, worldY);
	initStone();
	initWater();
	initClouds();
	initPlants();
	if (!loadNextLevel()) {
		reloadLevel();
	}
	scheduleEvent(getDjTrack(RAINSND), rain, 0, rainInterval);
	scheduleEvent(-1, breeze, 0, rainInterval/2);
	scheduleEvent(getDjTrack(DIRTSND), dirtFlow, 0, spreadInterval);
	scheduleEvent(getDjTrack(HEATSND), ecoEvaporation, 0, evapInterval);
	scheduleEvent(getDjTrack(PLANTSND), plantsAction, 0, plantsInterval);
	//baseGardenLevel();
	//generateLevel(GM.level);
	setViewDimension(worldX, worldY);
	setViewPosition(worldX/2, worldY/2);
}

void islandLevel() {
	World *w = getWorld();
	int spawnPos[2] = {worldX/2, worldY/2};
	makeCloud();
	placeCircle(placeClouds, spawnPos[0], spawnPos[1] + 15, 8);
	makeSnake(spawnPos[0], spawnPos[1]);
}

void baseGardenLevel() {
	World *w = getWorld();
	int spawnPos[2] = {worldX/2, worldY/2};
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			if (!checkFormID(x, y, WATER)) {
				//placeDirt(x, y);
				if (!checkFormID(x, y, STONE)) {
					if (randPercent() < grassChance) {
						placeGrass(x, y);
					}
					if (randPercent() < flowerChance) {
						placeFlower(x, y);
					}
				}
			}
		}
	}
	makeSnake(spawnPos[0], spawnPos[1]);

}

void placeCircle(Form* (*place)(int,int), int xPos, int yPos, int radius) {
	World *w = getWorld();
	for (int x = -radius; x < radius; x++) {
		for (int y = -radius; y < radius; y++) {
			int curX = xPos + x;
			int curY = yPos + y;
			if (distance(curX, curY, xPos, yPos) < radius) {
				int cx = (curX % w->x + w->x) % w->x;
				int cy = (curY % w->y + w->y) % w->y;
				place(cx, cy);
			}
		}
	}

}

void endGarden() {
	freeClouds();
	freeWater();
	freeStone();
	linkedList *curSnake = snakeList;
	while (curSnake) {
		freeSnake(curSnake->data);
		curSnake = curSnake->next;
	}
	freeListSaveObj(&snakeList);
	snakeCount = 0;
	snakeList = 0;
}

