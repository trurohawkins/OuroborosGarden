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
#include "stone.c"
#include "plant.c"
#include "snake.c"

typedef struct {
	Menu *curMenu;
	Menu *startMenu;
	Menu *pauseMenu;
} GameState;

GameState GM;

#include "garden.c"

void pressUp(void *guy, float val);
void pressLeft(void *guy, float val);
void pressDown(void *guy, float val);
void pressRight(void *guy, float val);
void pressSpace(void *, float val);

void pauseGame(void *, float val);
void resumeGame();

int main(int argc, char **argv) {
	if (argc > 1) {
		drawing = false;
	}
	startWorld(drawing, true);
	renderFunc = &renderGarden;
	Player *god = makePlayer(&GM, 0, 0);
	addPlayer(god);
	addControl(god, "K0W", pressUp);
	addControl(god, "K0A", pressLeft);
	addControl(god, "K0S", pressDown);
	addControl(god, "K0D", pressRight);
	addControl(god, "K0 ", pressSpace);
	addControl(god, "K0\x1B", pauseGame);

	Menu *startMenu = makeMenu(1, 2, 20, 10);
	Button *play = getButton(startMenu, 0, 1);
	nameButton(play, "play");
	play->func = &startGarden;

	Button *exit = getButton(startMenu, 0, 0);
	nameButton(exit, "exit");
	exit->func = &exitGame;
	selectButton(startMenu, 0, 1);
 	startMenu->pos[0] = 0.48f;
	startMenu->pos[1] = 0.48f;

	GM.startMenu = startMenu;

	Menu *pauseMenu = makeMenu(1, 3, 20, 10);
	play = getButton(pauseMenu, 0, 2);
	nameButton(play, "play");
	play->func = resumeGame;

	Button *restart = getButton(pauseMenu, 0, 1);
	nameButton(restart, "restart");

	exit = getButton(pauseMenu, 0, 0);
	nameButton(exit, "exit");
	exit->func = &exitGame;
 	pauseMenu->pos[0] = 0.48f; 
	pauseMenu->pos[1] = 0.3f;

	GM.pauseMenu = pauseMenu;

	GM.curMenu = startMenu;
	//makeGarden();
	quickExit = false;

	runWorld();

	if (GM.curMenu != startMenu) {
		endGarden();
	}
	endWorld();

	deleteMenu(pauseMenu);
	deleteMenu(startMenu);
	return 0;
}


void pressUp(void *gm, float val) {
	if (val == 1) { 
		GameState *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 0);
		}
	}
}

void pressLeft(void *gm, float val) {
	if (val == 1) { 
		GameState *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 1);
		}
	}
}

void pressDown(void *gm, float val) {
	if (val == 1) { 
		GameState *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 2);
		}
	}
}

void pressRight(void *gm, float val) {
	if (val == 1) { 
		GameState *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 3);
		}
	}
}

void pressSpace(void *gm, float val) {
	if (val == 1) {
		GameState *GM = gm;
		if (GM->curMenu) {
			pressButton(GM->curMenu);
		} else {
			toggleGamePause();
		}
	}
}

void pauseGame(void *gm, float val) {
	if (val == 1) {
		GameState *GM = gm;
		if (GM->curMenu == GM->startMenu) {
			exitGame();
		} else if (GM->curMenu == NULL) {
			GM->curMenu = GM->pauseMenu;
		} else {
			GM->curMenu = NULL;
		}
	}
}

void resumeGame() {
	GM.curMenu = NULL;
}

