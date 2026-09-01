#include "dirt.h"

//float dirtDry[3] = {203, 102, 12};
//float dirtDry[3] = {154, 94, 23};
float dirtDry[3] = {186, 91, 43};
float dirtWet[3] = {79, 30, 3};
float intake[2] = {0.02, 0.2};
float output[2] = {0.05, 0.1};

int dirtStats = 2;
#define MAXADDFORMS 4
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

Form *placeDirt(int x, int y) {
	if (!checkFormID(x, y, DIRT)) {
		Form *dirt = makeDirt();
		if (!placeForm(dirt, x, y)) {
			freeForm(dirt);
		} else {
			return dirt;
		}
	}
	return NULL;
}

void dirtCell(int x, int y) {
	World *w = getWorld();
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
					addEco(x, y, *src);
				}
			}
		}
	}
}

int dir = 0;

void dirtFlow(void*) {
	World *w = getWorld();
	if (dir == 0) {
		for (int x = 0; x < w->x; x++) {
			for (int y = 0; y < w->y; y++) {
				dirtCell(x, y);
			}
		}
	} else if (dir == 1) {
		for (int x = w->x-1; x >= 0; x--) {
			for (int y = w->y-1; y >= 0; y--) {
				dirtCell(x, y);
			}
		}
	} else if (dir == 2) {
			for (int y = 0; y < w->y; y++) {
		for (int x = 0; x < w->x; x++) {
				dirtCell(x, y);
			}
		}
	} else if (dir == 3) {
			for (int y = w->y-1; y >= 0; y--) {
		for (int x = w->x-1; x >= 0; x--) {
				dirtCell(x, y);
			}
		}
	}
	dir = (dir + 1) % 4;

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
	screenChanged(0, 0);
	return diff;
}


float addEco(int x, int y, float amnt) {
	if (equal(amnt, 0)) {
		return 0;
	}
	float startAmnt = amnt;
	Form *buff[MAXADDFORMS] = {0};
	dfsDirt(x, y, MAXADDFORMS, buff);
	for (int i = 0; i < MAXADDFORMS; i++) {
		if (buff[i]) {
			int dist = manhattanDistance(x, y, buff[i]->pos[0], buff[i]->pos[1]);
			float a = amnt / (2 * dist);
			a = changeEco(buff[i], a);
			if (amnt - a > 0) {
				amnt -= a;
			} else {
				amnt = 0;
				//if (equal(0, amnt) || amnt < 0) {
				break;
			}
		}
	}
	return startAmnt - amnt;
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
			float a = dist > 0 ? amnt / (2 * dist) : amnt;
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
	int pow = 4;
	Form *buff[pow] = {};
	dfsDirt(x, y, pow, buff);
	for (int i = 0; i < pow; i++) {
		if (buff[i]) {
			Form *soil = buff[i];
			float *eco = getStat(soil, ECO);
			if (eco) {
				if (*eco == 0) {
					*eco = evapMinimum;
				}
				int dist = manhattanDistance(x, y, buff[i]->pos[0], buff[i]->pos[1]);
				amnt /= 2 * dist;
				changeEco(soil, amnt);
			}
		}
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
					/*
						 float *eco = getStat(dirt, ECO);
						 if (*eco > evapMinimum) {
						 if (*eco + evap < evapMinimum) {
						 evap = *eco - evapMinimum;
						 }
						 */
					changeEco(dirt, evap);
					//}
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

void *renderDirt(void *data) {
	Form *dirt = data;
	float eco = *getStat(dirt, ECO);
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = DIRTLAYER,
		.pos = {
			.x = worldXToScreenX(dirt->pos[0]),// + screenX/2 - frameDim[0]/2;
			.y = worldYToScreenY(dirt->pos[1]),// + screenY/2 - frameDim[1]/2;
		},
	};
	Color col = {
		.rgb = {
			lerp(dirtDry[0], dirtWet[0], eco),
			lerp(dirtDry[1], dirtWet[1], eco),
			lerp(dirtDry[2], dirtWet[2], eco)
		}
	};
	memcpy(reco.data, &col, sizeof(Color));
	addRenderCommand(reco);
	return NULL;//commands;
}

Color dirtColor(Form *dirt) {
	float eco = *getStat(dirt, ECO);
	Color col = {
		.rgb = {
			lerp(dirtDry[0], dirtWet[0], eco),
			lerp(dirtDry[1], dirtWet[1], eco),
			lerp(dirtDry[2], dirtWet[2], eco)
		}
	};
	return col;
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
			Form *f = checkFormID(cur[0], cur[1], DIRT);//Stat(cur[0], cur[1], ECO);
			if (f) {// && f->id == DIRT) {
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

