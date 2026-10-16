#include "dj.h"

WorldDJ dj;

void initWorldDJ(int numTracks){
	dj.numTracks = numTracks;
	dj.tracks = calloc(dj.numTracks, sizeof(Track*));
}

void initDjTrack(int index, int sounds) {
	if (index >= 0 && index < dj.numTracks) {
		Track *t = calloc(1, sizeof(Track));
		t->sounds = calloc(sounds, sizeof(int));
		t->soundNum = sounds;
		for (int i = 0; i < sounds; i++) {
			t->sounds[i] = -1;
		}
		dj.tracks[index] = t;
	}
}

int addDjTrack(char *file, int index) {
	if (index >= 0 && index < dj.numTracks) {
		Track *t = dj.tracks[index];
		if (t) {
			for (int i = 0; i < t->soundNum; i++) {
				if (t->sounds[i] < 0) {
					size_t size = 100;
					char buff[size];
					memcpy(buff, "resources/", 10);
					memcpy(buff+10, file, min(size, strlen(file)+1));
					t->sounds[i] = processAudioFile(buff, false);
					t->curSound = t->sounds[i];
					return t->sounds[i];
				}
			}
		}
	}
	return -1;
}

int getDjTrack(int index) {
	if (index >= 0 && index < dj.numTracks) {
		return dj.tracks[index]->curSound;
	}
}

void setMuteTrack(int index, bool state) {
	setMute(dj.tracks[index]->curSound, state);
	dj.tracks[index]->muted = state;
}

bool getMute(int index) {
	return dj.tracks[index]->muted;
}

void setTrackSound(int index, int sound) {
	if (index >= 0 && index < dj.numTracks) {
		Track *t = dj.tracks[index];
		if (sound >= 0 && sound < t->soundNum) {
			if (t->curSound != t->sounds[sound]) {
				t->curSound = t->sounds[sound];
				setMute(t->curSound, t->muted);
				setSoundOnEvent(index, t->curSound);
			}
		}
	}
}

void freeTrack(Track *t) {
	if (t) {
		free(t->sounds);
		free(t);
	}
}

void freeWorldDJ(){
	if (dj.tracks) {
		for (int i = 0; i < dj.numTracks; i++) {
			freeTrack(dj.tracks[i]);
		}
		free(dj.tracks);
	}
}
