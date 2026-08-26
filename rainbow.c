#include "rainbow.h"

float ecoMax = 0.5;

Rainbow *newRainbow() {
	World *w = getWorld();
	Rainbow *r = calloc(1, sizeof(Rainbow));
	r->gotData = true;
	r->map = calloc(w->x, sizeof(bool*));
	for (int i = 0; i < w->x; i++) {
		r->map[i] = calloc(w->y, sizeof(bool));
	}
	return r;
}

void actualizeRainbow(Rainbow *r) {
	r->self = makeForm(RAINBOW);
	
	Nub *ren = growRenderNub(r->self);
	RenderObject *rob = ren->data;
	rob->data = r;
	rob->render = renderRainbow;

	Actor *actor = makeFormActor(r->self);
	Action *action = makeAction(0, rainbowAction, r);
	addAction(actor, action);
	addActor(actor);

	setRainbow(&r);
	r->colorOffset = randomInt(6);
	r->changeInterval = 30;
	r->changeTimer = 0;

	r->ecoAmount = ecoMax;
}

void placeRainbow(Rainbow *r) {
	int highX = -1;
	int lowX = 99999; // arbitrary high value
	int highY = -1;
	int lowY = 99999; // arbitrary high value
	intList *cur = r->full;
	while (cur) {
		int x = cur->data;
		int y = cur->next->data;
		if (!checkFormID(x, y, RAINBOW)) {
			placeForm(r->self, x, y);
		}

		if (x < lowX) {
			lowX = x;
		} else if (x > highX) {
			highX = x;
		}
		if (y < lowY) {
			lowY = y;
		} else if (y > highY) {
			highY = y;
		}
		cur = cur->next->next;
	}
	r->centerX = lowX + ((highX - lowX) / 2);
	r->centerY = lowY + ((highY - lowY) / 2);
}

void removeRainbow(Rainbow *r) {
	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			removeForm(r->self, x, y);
		}
	}
}

int rainbowAction(void *r, Action *a, float delta) {
	Rainbow *rb = (Rainbow*)a->data;
	//printf("rainbow action %p\n", rb);
	if (rb->changeTimer < rb->changeInterval) {
		rb->changeTimer++;
	} else {
		rb->changeTimer = 0;
		rb->colorOffset = (rb->colorOffset + 1) % 14;
		intList *cur = rb->full;
		while (cur) {
			int x = cur->data;
			int y = cur->next->data;
			addEco(x, y, rb->ecoAmount);

			cur = cur->next->next;
		}
	}
}

void recFill(Rainbow *r, int x, int y) {
	//printf("%p rec fill check %i, %i\n", r->map, x, y);
	if (r->map == 0) {
		return;
	}
	World *w = getWorld();
	if (x < 0 || y < 0 || x >= w->x || y >= w->y) {
		//printf("%i, %i out of bounds\n", x, y);
		freeRainbowData(r);
		return;
	}
	if (r->map[x][y] == 0) {
		// if there is already a rainbow get out of here!
		if (checkFormID(x, y, RAINBOW) && !checkFormID(x, y, SNAKE)) {
			freeRainbowData(r);
			return;
		}
		r->map[x][y] = 1;
		addToIntList(&(r->full), x);
		addToIntList(&(r->full), y);
		// if snake add to current border
		if(checkFormID(x, y, SNAKE)) {
			addToIntList(&(r->border), x);
			addToIntList(&(r->border), y);
			return;
		}
		//printf("adding %i, %i to rb %p\n", x, y, r);
		recFill(r, x + 1, y);
		recFill(r, x - 1, y);
		recFill(r, x, y + 1);
		recFill(r, x, y - 1);
		recFill(r, x + 1, y + 1);
		recFill(r, x + 1, y - 1);
		recFill(r, x - 1, y + 1);
		recFill(r, x - 1, y - 1);
	}
}

Rainbow *fill(int startX, int startY) {
	Rainbow *r = 0;
	if (!checkFormID(startX, startY, SNAKE) && !checkFormID(startX, startY, RAINBOW)) {
		//printf("chcking %i, %i\n", startX, startY);
		r = newRainbow();
		recFill(r, startX, startY);
		if (r->map != 0) {
			actualizeRainbow(r);
			//printf("got rainbow\n");
			placeRainbow(r);
		} else {
			freeRainbow(r);
			r = 0;
		}
	}
	return r;
}

void *renderRainbow(void *data) {
	Rainbow *r = data;
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = RAINBOWLAYER,
	};
	intList *cur = r->full;
	while (cur) {
		int x = cur->data;
		int y = cur->next->data;
		float d = max(abs(x - r->centerX), abs(y - r->centerY));
		int place = ((int)(r->colorOffset + d) % 14) * 3;
		PosColor pc = {
			.pos = {
				.x = worldXToScreenX(x),
				.y = worldYToScreenY(y),
			},
			.color = {
				r->colors[place], r->colors[place+1], r->colors[place+2]
			},
		};
		memcpy(reco.data, &pc, sizeof(PosColor));
		addRenderCommand(reco);
		cur = cur->next->next;
	}
}

bool checkRainbow(void *r) {
	Rainbow *rb = r;
	bool check = checkBorder(r);
	return check;
}

bool checkBorder(Rainbow *r) {
	intList *cur = r->border;
	World *w = getWorld();
	while (cur != 0) {
		int x = cur->data;
		cur = cur->next;
		int y = cur->data;
		if (!checkFormID(x, y, SNAKE)) {
			removeRainbow(r);
			freeRainbowData(r);
			r->map = calloc(w->x, sizeof(bool*));
			for (int i = 0; i < w->x; i++) {
				r->map[i] = calloc(w->y, sizeof(bool));
			}
			recFill(r, x, y);
			if (r->map == 0) {// || r->fillCount <= 1) {
				//freeRainbow(r);
				return true;
			} else {
				placeRainbow(r);
				//refresh eco when expanding rainbow
				//r->ecoAmount = ecoMax;
				return false;
			}
		}
		cur = cur->next;
	}
	return false;
}

void setRainbow(Rainbow **r) {
	Rainbow *rb = *r;
	memcpy(rb->colors, rainbowColors, 42 * sizeof(float));
	//rb->colors = calloc(42, sizeof(float));
	/*
	float size = sizeof(float) * 3;

	float red[3] = {150.0, 15.0, 28.0};
	memcpy(rb->colors, red, size);
	float redO[3] = {187.0, 0, 0};
	memcpy(rb->colors+3, redO, size);

	float orange[3] = {196.0, 71.0, 20.0};
	memcpy(rb->colors+6, orange, size);
	float orangeY[3] = {226.0, 99.0, 5.0};
	memcpy(rb->colors+9, orangeY, size);

	float yellow[3] = {225.0, 145.0, 15.0};
	memcpy(rb->colors+12, yellow, size);
	float yellowG[3] = {241.0, 195.0, 11.0};
	memcpy(rb->colors+15, yellowG, size);

	float green[3] = {134.0, 177.0, 0};
	memcpy(rb->colors+18, green, size);
	float greenB[3] = {16.0, 141.0, 9.0};
	memcpy(rb->colors+21, greenB, size);
	
	float blue[3] = {4.0, 163.0, 114.0};
	memcpy(rb->colors+24, blue, size);
	float blueI[3] = {17.0, 121.0, 126.0};
	memcpy(rb->colors+27, blueI, size);

	float indigo[3] = {8.0, 25.0, 141.0};
	memcpy(rb->colors+30, indigo, size);
	float indigoV[3] = {69.0, 5.0, 174.0};
	memcpy(rb->colors+33, indigoV, size);

	float violet[3] = {114.0, 0, 136.0};
	memcpy(rb->colors+36, violet,  size);
	float violetR[3] = {124.0, 12.0, 87.0};
	memcpy(rb->colors+39, violetR,  size);
	*/
}

float rainbowColors[42] = {
	//red
	150.0, 15.0, 28.0,
	//red orange
	187.0, 0, 0,
	//orange
	196.0, 71.0, 20.0,
	//orange yellow
	226.0, 99.0, 5.0,
	//yellow
	225.0, 145.0, 15.0,
	//yellow green
	241.0, 195.0, 11.0,
	//green
	134.0, 177.0, 0,
	//green blue
	16.0, 141.0, 9.0,
	//blue
	4.0, 163.0, 114.0,
	//blue indigo
	17.0, 121.0, 126.0,
	//indigo
	8.0, 25.0, 141.0,
	//indigo violet
	69.0, 5.0, 174.0,
	//violet
	114.0, 0, 136.0,
	//viloet red
	124.0, 12.0, 87.0,
};

void freeRainbowData(Rainbow *r) {
	//printf("free rainbow data\n");
	World *w = getWorld();
	if (r->map != 0) {
		for (int i = 0; i < w->x; i++) {
			free(r->map[i]);
		}
		free(r->map);
		r->map = 0;
	}

	if (r->border != 0) {
		freeIntList(&(r->border));
		r->border = 0;
	}
	if (r->full != 0) {
		freeIntList(&(r->full));
		r->full = 0;
	}
	r->gotData = false;
}

void freeRainbow(void *r) {
	//printf("free the rainbow %p\n", r);
	Rainbow *rb = r;
	removeRainbow(rb);
	if (rb->self) { // in case rainbow hasnt been actualized
		Actor *a = findNub(rb->self, 2)->data;
		a->deleteMe = true;
		freeForm(rb->self);
	}
	freeRainbowData(rb);
	free(rb);
}


