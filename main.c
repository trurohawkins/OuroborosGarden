#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

linkedList *snakeList;
bool drawing = true;
#include "constants.h"
#include "help.c"
#include "dj.c"
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
	initCookBook(3, 4, 1);
	cookBook.recipes[0] = (FormRecipe){"Dirt", placeDirt, removeForm, freeForm};
	cookBook.recipes[1] = (FormRecipe){"Stone", placeStone, removeFromMass, deleteStone};
	cookBook.recipes[2] = (FormRecipe){"Water", placeWater, removeFromMass, deleteWater};
	cookBook.infos[3] = (NubInfo){"Plant", inspectPlant};
	cookBook.names[0] = (StatName){"Eco", 0};
	initWorldDJ(6);
	startWorld(drawing, true);
	setRenderStride(2, 1);
	initDjTrack(RAINSND, 1);
	addDjTrack("Wuh.wav", RAINSND);
	initDjTrack(DIRTSND, 1);
	addDjTrack("foop.wav", DIRTSND);
	initDjTrack(HEATSND, 1);
	addDjTrack("shaa.wav", HEATSND);
	initDjTrack(PLANTSND, 2);
	addDjTrack("Baboop.wav", PLANTSND);
	addDjTrack("grrr.wav", PLANTSND);
	setTrackSound(PLANTSND, 0);
	initDjTrack(SNAKESND, 1);
	addDjTrack("fftt.wav", SNAKESND);
	initDjTrack(POOPSND, 1);
	addDjTrack("fart.wav", POOPSND);
	for (int i = 0; i < POOPSND; i++) {
		if (i != PLANTSND) {
			setMuteTrack(i, true);
		}
	}
	makeMenus();
	addRenderFunction(renderGM);
	
	//makeGarden();
	quickExit = false;
	runWorld();

	if (GM.curMenu != GM.startMenu) {
		endGarden();
	}
	endWorld();
	freeWorldDJ();
	freeMenus();
	return 0;
}
