#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>
#include "stat.c"

linkedList *snakeList;
bool drawing = true;
#include "constants.h"
#include "dirt.c"
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

int main() {
	startWorld(drawing, true);
	Player *god = makePlayer(0, 0, 0);
	addPlayer(god);
	addControl(god, "K0 ", pressPause);

	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			placeForm(makeDirt(), x, y);
			if (randPercent() > 0.95f) {
				placeFlower(x, y);
			}
		}
	}

	//placeGrass(10, 30);
	placeFlower(45, 20);

	initPlants();

	Snake *snake0 = makeSnake(worldX/2, worldY/2);

	runWorld();

	linkedList *curSnake = snakeList;
	while (curSnake) {
		freeSnake(curSnake->data);
		curSnake = curSnake->next;
	}
	freeListSaveObj(&snakeList);

	endWorld();
	return 0;
}
