#include "garden.h"

int spawnPos[2] = {0, 0};

void makeGarden() {
	GM.curMenu = NULL;
	//makeWorld(worldX, worldY);
	initStone();
	initWater();
	initClouds();
	initPlants();
	setViewDimension(worldX, worldY);
	setViewPosition(worldX/2, worldY/2);
	debugWrite("making garden\n");
	if (!loadNextLevel()) {
		reloadLevel();
	}
	startGarden();
	//setMuteTrack(PLANTSND, true);
	//baseGardenLevel();
	//generateLevel(GM.level);
}

void startGarden() {
	startDirt();
	startPlants();
	startClouds();
}

void islandLevel() {
	World *w = getWorld();
	spawnPos[0] = worldX/2;
	spawnPos[1] = worldY/2;
	makeCloud();
	placeCircle(placeClouds, spawnPos[0], spawnPos[1] + 15, 8);
	debugWrite("level built\n");
	makeSnake(spawnPos[0], spawnPos[1]);
}

void baseGardenLevel() {
	World *w = getWorld();
	spawnPos[0] = worldX/2;
	spawnPos[1] = worldY/2;
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

