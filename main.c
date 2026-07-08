#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>
#include "stat.c"

linkedList *snakeList;
#include "constants.h"
#include "dirt.c"
#include "snake.c"

int main() {
	startWorld(true, true);

	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			placeForm(makeDirt(), x, y);
		}
	}

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
