#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

linkedList *snakeList;
bool drawing = true;
#include "constants.h"
#include "stat.c"
#include "help.c"
#include "mass.c"
#include "floaty.c"
#include "dirt.c"
#include "water.c"
#include "stone.c"
#include "plant.c"
#include "rainbow.c"
#include "clouds.c"
#include "poop.c"
#include "snake.c"
#include "GM.c"
#include "garden.c"


int main(int argc, char **argv) {
	if (argc > 1) {
		drawing = false;
	}
	startWorld(drawing, true);
	renderFunc = &renderGarden;
	setRenderStride(2, 1);
	makeMenus();
	
	//makeGarden();
	quickExit = false;
	runWorld();

	if (GM.curMenu != GM.startMenu) {
		endGarden();
	}
	endWorld();
	freeMenus();
	return 0;
}
