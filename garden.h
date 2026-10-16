#pragma once

void makeGarden();
void endGarden();
void islandLevel();
void baseGardenLevel();
void generateLevel(int level);

void placeCircle(Form *(*place)(int, int), int xp, int yp, int radius);

