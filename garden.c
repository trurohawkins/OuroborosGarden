#include "garden.h"

void makeGarden() {
	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	World *w = getWorld();
	initWater();
	initStone();

	for (int i = 0; i < 3; i++) {
		int xp = randRange(0, worldX);
		int yp = randRange(0, worldY);
		int radius = randRange(4, 6);
		//placeCircle(placeWater, xp, yp, radius);
	}

	for (int i = 0; i < 8; i++) {
		int xp = randRange(0, worldX);
		int yp = randRange(0, worldY);
		int radius = randRange(4, 5);
		//placeCircle(placeStone, xp, yp, radius);
	}

	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			if (!checkFormID(x, y, WATER)) {
				placeForm(makeDirt(), x, y);
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

	initPlants();

	makeSnake(worldX/2, worldY/2);
}

void placeCircle(void (*place)(int,int), int xPos, int yPos, int radius) {
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

void startGarden() {
	GM.curMenu = NULL;
	makeGarden();
}

void renderGarden() {
	if (GM.curMenu) {
		addMenu(GM.curMenu);
	} else {
		renderWorld();
	}
}

void endGarden() {
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

