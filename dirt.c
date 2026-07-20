#include "dirt.h"

float dirtA[3] = {179.0, 89.0, 23.0};
float dirtB[3] = {74.8, 22.4, 2.0};
float intake[2] = {0.01, 0.1};
float output[2] = {0.05, 0.1};

int dirtStats = 2;
#define MAXADDFORMS 8
#define MAXPULLFORMS 32

Form *makeDirt() {
	Form *dirt = makeForm(DIRT);
	initStats(dirt, dirtStats);
	addStat(dirt, ECO, 0);
	addStat(dirt, OUTPUT, 1);
	//Sigil *skin = createSigil(dirt)->data;
	Nub *ren = growRenderNub(dirt);
	RenderObject *rob = ren->data;
	rob->data = dirt;
	rob->render = renderDirt;
	//dirtColor(dirt);
	return dirt;
}

float checkSpread(int x, int y, float amnt) {
	Form *f = checkStat(x, y, ECO);
	if (f) {
		float *check = getStat(f, ECO);
		if (check) {
			if ((amnt > 0 && *check < maxEco) || (amnt < 0 && *check > 0)) {
				amnt = changeEco(f, amnt);
			}
		}
	}
	return amnt;
}

void addEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return;
	}

	World *w = getWorld(); 
	Form *buff[MAXADDFORMS] = {0};
	dfsDirt(x, y, MAXADDFORMS, buff);
	float pulled = 0;
	for (int i = 0; i < MAXADDFORMS; i++) {
		if (buff[i]) {
			float *eco = getStat(buff[i], ECO);
			amnt = changeEco(buff[i], amnt);
			if (equal(0, amnt) || amnt < 0) {
				break;
			}
		}
	}
}
float pullEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return 0;
	}
	Form *buff[MAXPULLFORMS];
	dfsDirt(x, y, MAXPULLFORMS, buff);
	float pulled = 0;
	for (int i = 0; i < MAXPULLFORMS; i++) {
		if (!drawing) {printf("buff form %p\n", buff[i]);}
		if (buff[i]) {
			float *eco = getStat(buff[i], ECO);
			if (!drawing) {printf("pulling from %f with eco %f\n", buff[i], *eco);}
			pulled += changeEco(buff[i], -amnt);
			if (!drawing) {printf("now eco is %f\n", *eco);}
			if (pulled >= amnt) {
				break;
			}
		}
	}
	return pulled;
}

float changeEco(Form *f, float amnt) {
	float *eco = getStat(f, ECO);
	float diff = fabs(amnt);
	if (eco) {
		if (*eco + amnt > maxEco) {
			diff = (*eco + amnt) - maxEco;
			*eco = 1;
		} else if (*eco + amnt < 0) {
			diff = *eco;
			*eco = 0;
		} else {
			*eco = clampF(*eco + amnt, 0, maxEco);
		}
		//setStat(f, "intake", calcIntake(*eco));
		//dirtColor(f);
		if (amnt > 0 && *eco > maxEco * 0.75f) {
			if (randPercent() > 0.95f) {
				//placeGrass(f->pos[0], f->pos[1]);
			}
		}
	}
	return diff;
}

void calcFlow(int x, int y) {
	Form *dirt = 0;
	float roots = 0;
	Cell *c = getCell(x, y);
	if (c) {
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i]) {
				Form *f = c->within[i];
				if (f->id == DIRT) {
					dirt = f;
				} else {
					float *r = getStat(f, ROOTS);
					if (r) {
						roots += *r;
					}
				}
			}
		}
		if (dirt) {
			setStat(dirt, OUTPUT, lerp(output[0], output[1], roots));
		}
	}
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

void *renderDirt(void *data) {
	Form *dirt = data;
	float eco = *getStat(dirt, ECO);
	RenderCommand reco = {
		.screenPos[0] = worldXToScreenX(dirt->pos[0]),// + screenX/2 - frameDim[0]/2;
		.screenPos[1] = worldYToScreenY(dirt->pos[1]),// + screenY/2 - frameDim[1]/2;
		.sigil = -1,
		.r = lerp(dirtA[0], dirtB[0], eco),
		.g = lerp(dirtA[1], dirtB[1], eco),
		.b = lerp(dirtA[2], dirtB[2], eco),
		.layer = DIRTLAYER,
	};
	addRenderCommand(reco);
	return NULL;//commands;
}

Form *checkSoil(int x, int y) {
	Cell *c = getCell(x, y);
	if (c) {
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i]) {
				if (c->within[i]->id == DIRT) {
					return c->within[i];
				}
			}
		}
	}
	return 0;
}

void dfsDirt(int x, int y, int max, Form **buff) {
	World *w = getWorld();
	int count = 0;
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
			Form *f = checkStat(cur[0], cur[1], ECO);
			if (f && f->id == DIRT) {
				buff[count] = f;
				if (!drawing) {printf("got form %p\n", f);}
				if (count + 1 < max) {
					count++;
				} else {
					return;
				}
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
}
