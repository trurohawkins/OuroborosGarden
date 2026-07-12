#include "dirt.h"

float dirtA[3] = {179.0, 89.0, 23.0};
float dirtB[3] = {74.8, 22.4, 2.0};
float intake[2] = {0.01, 0.1};
float output[2] = {0.05, 0.1};

int dirtStats = 2;

Form *makeDirt() {
	Form *dirt = makeForm(DIRT);
	initStats(dirt, dirtStats);
	addStat(dirt, ECO, 0);

	Sigil *skin = createSigil(dirt)->data;
	
	dirtColor(dirt);
	return dirt;
}

float checkSpread(int x, int y, float amnt) {
	Form *f = checkStat(x, y, ECO);
	if (f) {
		float *check = getStat(f, ECO);
		if (check && *check < maxEco) {
			amnt = changeEco(f, amnt);
		}
	}
	return amnt;
}

void addEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return;
	}
	World *w = getWorld(); 
	float extra = 0;
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		float queue[w->x * w->y * 2];
		// if worldx and world y bigger than 100, use dynamic allocation
		bool visited[w->x][w->y];
		memset(visited, 0, sizeof(visited));
		//add x y 
		queue[0] = x;
		queue[1] = y;
		visited[x][y] = true;
		//font 0 back 0
		int front = 0;
		int back = 2;
		while (front < back) {
			int cur[2] = {queue[front], queue[front+1]};
			amnt = checkSpread(cur[0], cur[1], amnt); 
			amnt -= spreadDither;
			if (equal(0, amnt) || amnt < 0) {
				break;
			}
			front += 2;
			int index = randomInt(8);
			int dir = coinFlip(-1, 1);
			for (int i = 0; i < 8; i++) {
				int *d = getDir8(index);
				int nx = cur[0] + d[0];
				int ny = cur[1] + d[1];
				if (nx > -1 && nx < w->x && ny > -1 && ny < w->y) {
					if (!visited[nx][ny]) { 
						queue[back++] = nx;
						queue[back++] = ny;
						visited[nx][ny] = true;
					}
				}
				if (dir > 0) {
					index = (index+1) % 8;
				} else {
					if (index > 0) {
						index--;
					} else {
						index = 7;
					}
				}
			}
		}
	}
	return;
}


float changeEco(Form *f, float amnt) {
	float *eco = getStat(f, ECO);
	float diff = 0;
	if (eco) {
		if (*eco + amnt > maxEco) {
			diff = (*eco + amnt) - maxEco;
			*eco = 1;
		} else {
			*eco = clampF(*eco + amnt, 0, maxEco);
		}
		//setStat(f, "intake", calcIntake(*eco));
		dirtColor(f);
		if (amnt > 0 && *eco > maxEco * 0.75f) {
			if (randPercent() > 0.95f) {
				//placeGrass(f->pos[0], f->pos[1]);
			}
		}
	}
	return diff;
}

void dirtColor(Form *f) {
	float eco = *getStat(f, ECO);
	Sigil *skin = findNub(f, 1)->data;
	skin->r = lerp(dirtA[0], dirtB[0], eco);
	skin->g = lerp(dirtA[1], dirtB[1], eco);
	skin->b = lerp(dirtA[2], dirtB[2], eco);
	char buffer[100];
	sprintf(buffer, "%f = dirt: %i, %i, %i\n", eco, skin->r, skin->g, skin->b);
	//debugWrite(buffer);
}

Form *checkSoil(int x, int y) {
	World *w = getWorld();
	Cell *c = &w->map[(y*w->x)+x];
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (c->within[i] && c->within[i]->id == DIRT) {
			return c->within[i];
		}
	}
	return 0;
}

