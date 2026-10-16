#pragma once

typedef struct {
	int *sounds;
	int soundNum;
	int curSound;
	bool muted;
} Track;

typedef struct {
	Track **tracks;
	int numTracks;
} WorldDJ;

void initWorldDJ(int numTracks);
void initDjTrack(int index, int sounds);
int addDjTrack(char *file, int index);
int getDjTrack(int index);
void setMuteTrack(int index, bool state);
bool getMute(int index);
void setTrackSound(int index, int sound);
void freeTrack(Track *t);
void freeWorldDJ();
