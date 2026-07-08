#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#define SOIL 0
#define SNAKE 1

#include "snake.c"

int worldX = 80;
int worldY = 40;

int main() {
	startWorld(true);

	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	Snake *snake0 = makeSnake(worldX/2, worldY/2);
	placeSnake(snake0);
	runWorld();
	removeSnake(snake0);
	freeSnake(snake0);

	endWorld();
	return 0;
}
