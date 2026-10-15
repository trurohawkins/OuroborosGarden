#pragma once
typedef struct {
	int *tracks;
	int numTracks;
} WorldDJ;

void initWorldDJ(int numTracks);
int addDjTrack(char *file, int index);
int getDjTrack(int index);
void freeWorldDJ();
