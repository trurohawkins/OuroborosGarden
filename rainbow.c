#include "rainbow.h"

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
	/*
	Actor *actor = makeActor(r->self);
	Action *action = makeAction(rainbowAction, t);
	addAction(actor, action);
	addActor(actor);
	*/
}

void placeRainbow(Rainbow *r) {
	intList *cur = r->full;
	while (cur) {
		int x = cur->data;
		int y = cur->next->data;
		if (!checkFormID(x, y, RAINBOW)) {
			placeForm(r->self, x, y);
		}
		cur = cur->next->next;
	}
}

void removeRainbow(Rainbow *r) {
	World *w = getWorld();
	for (int x = 0; x < w->x; x++) {
		for (int y = 0; y < w->y; y++) {
			removeForm(r->self, x, y);
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
		PosColor pc = {
			.pos = {
				.x = worldXToScreenX(x),
				.y = worldYToScreenY(y),
			},
			.color = {
				255, 255, 255
			},
		};
		memcpy(reco.data, &pc, sizeof(PosColor));
		addRenderCommand(reco);
		cur = cur->next->next;
	}
}

bool checkRainbow(void *r) {
	Rainbow *rb = r;
	debugWrite("checking rainbow\n");
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
		/*
		Actor *a = rb->self->actor;
		removeActor(rb->self->actor);
		deleteActor(a);
		free(rb->colors);
		freeAnim(rb->anim);
		*/
		freeForm(rb->self);
	}
	freeRainbowData(rb);
	free(rb);
}


