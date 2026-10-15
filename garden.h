#pragma once

void makeGarden();
void startGarden();
void endGarden();
void baseGardenLevel();
void generateLevel(int level);

void placeCircle(Form *(*place)(int, int), int xp, int yp, int radius);

