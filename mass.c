#include "mass.h"

Mass *makeMass(int id) {
	Mass *m = calloc(1, sizeof(Mass));
	m->self = makeForm(id);
	m->placed = true;
	return m;
}

Form *addToMass(Mass *m, int x, int y) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		int *pos = calloc(2, sizeof(int));
		pos[0] = x;//(x % w->x + w->x) % w->x;
		pos[1] = y;//(y % w->y + w->y) % w->y;
		addToList(&m->body, pos);
		placeForm(m->self, pos[0], pos[1]);
		return m->self;
	}
	return NULL;
}

bool chkPos(void *a, void *b) {
	int *aPos = a;
	int *bPos = b;
	return aPos[0] == bPos[0] && aPos[1] == bPos[1];
}

bool removeFromMass(Mass *m, int x, int y) {
	int pos[2] = {x, y};
	if (deleteFromListCompare(&m->body, pos, chkPos)) {
		removeForm(m->self, x, y);
		return true;
	}
	return false;
}

void removeMass(Mass *m) {
	for (linkedList *cur = m->body; cur != 0; cur = cur->next) {
		if (cur->data) {
			int *pos = cur->data;
			removeForm(m->self, pos[0] , pos[1]);
		}
	}
	m->placed = false;
}
void placeMass(Mass *m) {
	for (linkedList *cur = m->body; cur != 0; cur = cur->next) {
		if (cur->data) {
			int *pos = cur->data;
			placeForm(m->self, pos[0] , pos[1]);
		}
	}
	m->placed = true;
}


void moveMass(Mass *m, int xd, int yd) {
	removeMass(m);
	for (linkedList *cur = m->body; cur != 0; cur = cur->next) {
		if (cur->data) {
			int *pos = cur->data;
			incPos(pos, pos + 1, xd, yd);
		}
	}

	placeMass(m);
}

void renderMass(Mass *m, RenderCommand reco) {
	if (!m->placed) {
		return;
	}
	linkedList *cur = m->body;
	while (cur) {
		int *pos = cur->data;
		reco.pos.x = worldXToScreenX(pos[0]);
		reco.pos.y = worldYToScreenY(pos[1]);
		addRenderCommand(reco);
		cur = cur->next;
	}
}

void freeMass(void *mass) {
	Mass *m = mass;
	if (m->body) {
		linkedList *cur = m->body;
		while (cur) {
			int *pos = cur->data;
			removeForm(m->self, pos[0], pos[1]);
			cur = cur->next;
		}
		freeList(&m->body);
	}
	freeForm(m->self);
	free(m);
}

