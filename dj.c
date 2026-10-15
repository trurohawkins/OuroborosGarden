#include "dj.h"

WorldDJ dj;

void initWorldDJ(int numTracks){
	dj.numTracks = numTracks;
	dj.tracks = calloc(dj.numTracks, sizeof(int));
	for (int i = 0; i < numTracks; i++) {
		dj.tracks[i] = -1;
	}
}

int addDjTrack(char *file, int index) {
	if (index >= 0 && index < dj.numTracks) {
		if (dj.tracks[index] < 0) {
			debugWrite("making the track\n");
			size_t size = 100;
			char buff[size];
			memcpy(buff, "resources/", 10);
			memcpy(buff+10, file, min(size, strlen(file)+1));
			debugWrite(buff);
			debugWrite("\n");
			dj.tracks[index] = processAudioFile(buff, false);
		}
		return dj.tracks[index];
	}
	return -1;
}

int getDjTrack(int index) {
	if (index >= 0 && index < dj.numTracks) {
		return dj.tracks[index];
	}
}

void freeWorldDJ(){
	if (dj.tracks) {
		free(dj.tracks);
	}
}
