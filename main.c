#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

linkedList *snakeList;
bool drawing = true;
#include "constants.h"
#include "stat.c"
#include "help.c"
#include "dirt.c"
#include "water.c"
#include "plant.c"
#include "snake.c"

void pressPause(void *, float val) {
	if (val == 0) {
		toggleGamePause();
		for (linkedList *cur = snakeList; cur != 0; cur = cur->next) {
			Snake *s = cur->data;
		}
	}
}

int main(int argc, char **argv) {
	if (argc > 1) {
		drawing = false;
	}
	startWorld(drawing, true);
	Player *god = makePlayer(0, 0, 0);
	addPlayer(god);
	addControl(god, "K0 ", pressPause);
	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	World *w = getWorld();
	initWater();
	int radius = 4;
	for (int x = -radius; x < radius; x++) {
		for (int y = -radius; y < radius; y++) {
			int xp = worldX/2 + x;
			int yp = worldY/2 + y;
			if (distance(xp, yp, worldX/2, worldY/2) < radius) {
				placeWater(xp, yp);
			}
		}
	}
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			if (!checkFormID(x, y, WATER)) {
				placeForm(makeDirt(), x, y);
				if (randPercent() < grassChance) {
					placeGrass(x, y);
				}
				if (randPercent() < flowerChance) {
					placeFlower(x, y);
				}
			}
		}
	}

	initPlants();

	Snake *snake0 = makeSnake(worldX/2, worldY/2);

	runWorld();

	freeWater();
	linkedList *curSnake = snakeList;
	while (curSnake) {
		freeSnake(curSnake->data);
		curSnake = curSnake->next;
	}
	freeListSaveObj(&snakeList);
	endWorld();
	return 0;
}
