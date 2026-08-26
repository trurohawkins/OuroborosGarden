#pragma once

typedef struct {
	Form *self;
	bool** map;
	bool gotData;
	intList *border;
	intList *full;

	int centerX;
	int centerY;
	float colors[42];
	int colorOffset;
	int changeInterval;
	int changeTimer;
	float ecoAmount;
} Rainbow;

Rainbow *newRainbow();
void actualizeRainbow(Rainbow *r);
void placeRainbow(Rainbow *r);
void removeRainbow(Rainbow *r);
int rainbowAction(void *r, Action *a, float delta);

void recFill(Rainbow *r, int x, int y);
Rainbow *fill(int startX, int startY);
void *renderRainbow(void *data);
bool checkRainbow(void *r);
bool checkBorder(Rainbow *r);
void setRainbow(Rainbow **r);

void freeRainbowData(Rainbow *r);
void freeRainbow(void *r);

extern float rainbowColors[42];
