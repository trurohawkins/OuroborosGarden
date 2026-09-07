#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

linkedList *snakeList;
bool drawing = true;
#include "constants.h"
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
	initCookBook(1);
	cookBook.recipes[0] = (FormRecipe){"Dirt", makeDirt, freeForm};
	startWorld(drawing, true);
	makeEditor();
	setRenderStride(2, 1);
	makeMenus();
	addRenderFunction(renderGM);
	
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
