#pragma once

void makeGarden();
void startGarden();
void renderGarden();
void endGarden();
void generateLevel(int level);

void placeCircle(Form *(*place)(int, int), int xp, int yp, int radius);

