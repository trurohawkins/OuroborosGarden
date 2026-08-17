#pragma once

typedef struct {
	Form *self;
	bool** map;
	bool gotData;
	intList *border;
	intList *full;

} Rainbow;

Rainbow *newRainbow();
void actualizeRainbow(Rainbow *r);
void placeRainbow(Rainbow *r);
void removeRainbow(Rainbow *r);

void recFill(Rainbow *r, int x, int y);
Rainbow *fill(int startX, int startY);
void *renderRainbow(void *data);
bool checkRainbow(void *r);
bool checkBorder(Rainbow *r);

void freeRainbowData(Rainbow *r);
void freeRainbow(void *r);

