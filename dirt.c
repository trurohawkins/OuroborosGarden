#include "dirt.h"

float dirtA[3] = {179.0, 89.0, 23.0};
float dirtB[3] = {74.8, 22.4, 2.0};
float intake[2] = {0.0, 0.2};
float output[2] = {0.05, 0.1};

int dirtStats = 2;
#define MAXADDFORMS 64
#define MAXPULLFORMS 4

Form *makeDirt() {
	Form *dirt = makeForm(DIRT);
	initStats(dirt, dirtStats);
	addStat(dirt, ECO, 0);//randPercent());
	addStat(dirt, OUTPUT, 1);
	//Sigil *skin = createSigil(dirt)->data;
	Nub *ren = growRenderNub(dirt);
	RenderObject *rob = ren->data;
	rob->data = dirt;
	rob->render = renderDirt;
	//dirtColor(dirt);
	return dirt;
}

void dirtFlow(void*) {
	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y * w->x) + x];
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				Form *f = c->within[i];
				if (f) {
					if (f->id == DIRT) {
						float *eco = getStat(f, ECO);
						if (eco && !equal(*eco, 0)) {
							int start = 0;//randomInt(4);
							for (int j = 0; j < 4; j++) {
								int p[2] = {x, y};
								int index = (start + j) % 4;
								int *d4 = getDir4(index);
								incPos(p, p+1, d4[0], d4[1]);
								spreadEco(f, p[0], p[1]);
							}
						}
					} else {
						float *src = getStat(f, SOURCE);
						if (src) {
							pushEco(x, y, *src);
						}
					}
				}
			}
		}
	}
}

void spreadEco(Form *from, int x, int y) {
	Cell *c = getCell(x, y);
	if (c) {
		float *source = getStat(from, ECO);
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			Form *to = c->within[i];
			if (to && to->id == DIRT) {
				float *eco = getStat(to, ECO);
				if (*source > *eco) {
					float flow = (*source - *eco) / 2;
					flow = changeEco(to, flow);
					changeEco(from, -flow);
				}
			}
		}
	}
}

//adds or subtracts eco, and returns how much was actually added or subtracted
float changeEco(Form *form, float amnt) {
	if (amnt == 0) {
		return 0;
	}
	float *eco = getStat(form, ECO);
	float diff = fabs(amnt);
	if (eco) {
		if (amnt > 0) {
			amnt = min(amnt, calcIntake(*eco));
		} else {
			amnt = min(amnt, *getStat(form, OUTPUT));
		}
		float start = *eco;
		*eco = clampF(*eco + amnt, 0, maxEco);
		diff = max(*eco, start) - min(*eco, start);
	}
	worldChanged = true;
	return diff;
}


void addEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return;
	}
	Form *buff[MAXADDFORMS] = {0};
	dfsDirt(x, y, MAXADDFORMS, buff);
	for (int i = 0; i < MAXADDFORMS; i++) {
		if (buff[i]) {
			int dist = manhattanDistance(x, y, buff[i]->pos[0], buff[i]->pos[1]);
			float a = amnt / (2 * dist);
			amnt = changeEco(buff[i], a);
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
		if (buff[i]) {
			int dist = manhattanDistance(x, y, buff[i]->pos[0], buff[i]->pos[1]);
			float a = amnt / (2 * dist);
			pulled = changeEco(buff[i], -a);
			if (pulled >= amnt) {
				break;
			}
		}
	}
	return pulled;
}

void pushEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return;
	}
	Form *buff[8];
	dfsDirt(x, y, 8, buff);
	for (int i = 0; i < 8; i++) {
		Form *soil = buff[i];
		float *eco = getStat(soil, ECO);
		if (*eco == 0) {
			*eco = evapMinimum;
		}
		int dist = manhattanDistance(x, y, buff[i]->pos[0], buff[i]->pos[1]);
		amnt /= 2 * dist;
		changeEco(soil, amnt);
	}
}

void ecoEvaporation(void *) {
	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			Cell *c = &w->map[(y * w->x) + x];
			Form *dirt = 0;
			float evap = -evaporation;
			//printf("evap\n");
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				Form *f = c->within[i];
				if (f) {
					if (f->id == DIRT) {
						dirt = f;
					} else {
						float *c = getStat(f, COVER);
						if (c) {
							if (evap + *c < 0) {
								evap += *c;
							} else {
								evap = 0;
								break;
							}
						}
					}
				}
			}
			if (dirt) {
				if (evap < 0) {
					float *eco = getStat(dirt, ECO);
					if (*eco + evap < evapMinimum) {
						evap = *eco - evapMinimum;
					}
					changeEco(dirt, evap);
				}
				/*
					 float *eco = getStat(dirt, "eco");
					 if (*eco == 0) {
					 float *bio = getStat(dirt, "bio");
					 if (*bio - bioEvap > 0) {
					 changeBio(x, y, -bioEvap);
					 } else {
					 changeBio(x, y, -(*bio));
					 }
					 }
					 */
			}
		}
	}
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
		.type = 0,
		.index = -1,
		.layer = DIRTLAYER,
	};
	PosColor pc = {
		.pos = {
			.x = worldXToScreenX(dirt->pos[0]),// + screenX/2 - frameDim[0]/2;
			.y = worldYToScreenY(dirt->pos[1]),// + screenY/2 - frameDim[1]/2;
		},
		.color = {
			lerp(dirtA[0], dirtB[0], eco),
			lerp(dirtA[1], dirtB[1], eco),
			lerp(dirtA[2], dirtB[2], eco)
		},
	};
	memcpy(reco.data, &pc, sizeof(PosColor));
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

float calcIntake(float eco) {
	return lerp(intake[0], intake[1], eco);
}

