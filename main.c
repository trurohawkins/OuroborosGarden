#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#define SOIL 0
#define SNAKE 1

linkedList *snakeList;
#include "snake.c"

int worldX = 80;
int worldY = 40;

int main() {
	startWorld(true, true);

	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

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
