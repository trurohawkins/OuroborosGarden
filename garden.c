#include "garden.h"

void makeGarden() {
	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	initStone();
	initWater();
	initPlants();

	generateLevel(GM.level);
}

void generateLevel(int level) {
	World *w = getWorld();
	int spawnPos[2] = {worldX/2, worldY/2};
	if (level == 0) {
		spawnPos[1] -= 10;
		placeCircle(placeWater, spawnPos[0], spawnPos[1], 6);
		int island[2] = {spawnPos[0] - 3, spawnPos[1] + 20};
		placeCircle(placeWater, island[0], island[1], 4);
		
		for (int i = -1; i < 4; i++) {
			int *dir = getDir4(i);
			int pos[2] = {island[0] + dir[0], island[1] + dir[1]};
			//removeForm(water->self, pos[0], pos[1]);
			removeFromMass(water, pos[0], pos[1]);
			Form *dirt = placeDirt(pos[0], pos[1]);
			float *eco = getStat(dirt, ECO);
			*eco = 1;
		}
		Form *flower = placeFlower(island[0], island[1]);
		grow(flower);
		grow(flower);
		float *eco = getStat(flower, ECO);
		*eco = 1;
		Plant *p = getPlant(flower);
		p->life = p->cycle - 4;
	} else if (level == 1) {
		placeCircle(placeWater, spawnPos[0], spawnPos[1], 4);

		placeCircle(placeWater, 0, 0, 5);
		placeCircle(placeStone, worldX/2, 0, 4);
		placeCircle(placeStone, 0, worldY/2, 4);

	} else if (level == 2) {
		placeFlower(30, 10);

	} else {
		for (int i = 0; i < 3; i++) {
			int xp = randRange(0, worldX);
			int yp = randRange(0, worldY);
			int radius = randRange(4, 6);
			placeCircle(placeWater, xp, yp, radius);
		}

		for (int i = 0; i < 8; i++) {
			int xp = randRange(0, worldX);
			int yp = randRange(0, worldY);
			int radius = randRange(4, 5);
			placeCircle(placeStone, xp, yp, radius);
		}

	}
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			if (!checkFormID(x, y, WATER)) {
				placeDirt(x, y);
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

void startGarden() {
	GM.curMenu = NULL;
	makeGarden();
}

void renderGarden() {
	if (GM.curMenu) {
		addMenu(GM.curMenu);
	}
	renderWorld();
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

