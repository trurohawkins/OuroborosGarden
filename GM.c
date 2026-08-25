#include "GM.h"

GameMaster GM  = {
	.level = 0,
	.curPlants = 0,
	.plantGoal = 400,
};

void makeMenus() {
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
	nameButton(play, "resume");
	play->func = &resumeGame;

	Button *restart = getButton(pauseMenu, 0, 1);
	nameButton(restart, "restart");
	restart->func = &restartGame;

	exit = getButton(pauseMenu, 0, 0);
	nameButton(exit, "exit");
	exit->func = &returnToMenu;

	pauseMenu->pos[0] = 0.48f; 
	pauseMenu->pos[1] = 0.3f;

	GM.pauseMenu = pauseMenu;

	GM.curMenu = startMenu;

	GM.winScreen = makeMenu(1, 1, 20, 20);
	Button *win = getButton(GM.winScreen, 0, 0);
	nameButton(win, "YOU WIN!!!");
	win->func = &loadLevel;
	GM.winScreen->pos[0] = 0.5;
	GM.winScreen->pos[1] = 0.5;

}

void plantCount(int amnt) {
	GM.curPlants += amnt;
}

void checkPlants() {
	if (GM.won) {
		return;
	}
	if (GM.curPlants >= GM.plantGoal) {
		GM.won = true;
		// win next level!!!
		GM.level++;
		GM.curMenu = GM.winScreen;
	}
}

void loadLevel() {
	endLevel();
	startGarden();
	GM.curPlants = 0;
	GM.won = 0;
}

void pressUp(void *gm, float val) {
	if (val == 1) { 
		GameMaster *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 0);
		}
	}
}

void pressLeft(void *gm, float val) {
	if (val == 1) { 
		GameMaster *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 1);
		}
	}
}

void pressDown(void *gm, float val) {
	if (val == 1) { 
		GameMaster *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 2);
		}
	}
}

void pressRight(void *gm, float val) {
	if (val == 1) { 
		GameMaster *GM = gm;
		if (GM->curMenu) {
			menuMoveCursor(GM->curMenu, 3);
		}
	}
}

void pressSpace(void *gm, float val) {
	if (val == 1) {
		GameMaster *GM = gm;
		if (GM->curMenu) {
			pressButton(GM->curMenu);
		} else {
			toggleGamePause();
		}
	}
}

void pauseGame(void *gm, float val) {
	if (val == 1) {
		GameMaster *GM = gm;
		if (GM->curMenu == GM->startMenu) {
			exitGame();
		} else if (GM->curMenu == NULL) {
			toggleGamePause();
			GM->curMenu = GM->pauseMenu;
		} else if (GM->curMenu != GM->winScreen) {
			resumeGame();
		}
		screenChanged(0, 0);
	}
}

void resumeGame() {
	toggleGamePause();
	GM.curMenu = NULL;
	screenChanged(0, 0);
}

void endLevel() {
	clearTimedEvents();
	endGarden();
	freeWorld();
	deleteActorLists();
}

void restartGame() {
	toggleGamePause();
	loadLevel();
}

void returnToMenu() {
	endLevel();
	toggleGamePause();
	GM.level = 0;
	GM.curPlants = 0;
	GM.curMenu = GM.startMenu;
	screenChanged(0, 0);

}

void freeMenus() {
	deleteMenu(GM.pauseMenu);
	deleteMenu(GM.startMenu);
}
