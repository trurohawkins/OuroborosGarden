#include "clouds.h"

linkedList *clouds = 0;

void initClouds() {
	if (!clouds) {
		clouds = makeList();
		addTimedEvent(breeze, 0, rainInterval/2);
		addTimedEvent(rain, 0, rainInterval);
	}
}

Mass *makeCloud() {
	Mass *cloud = makeMass(CLOUDS);
	Nub *ren = growRenderNub(cloud->self);
	RenderObject *rob = ren->data;
	rob->data = cloud;
	rob->render = renderClouds;
	addToList(&clouds, cloud);
	return cloud;
}

//default to latest cloud
Form *placeClouds(int x, int y) {
	if (clouds) {
		linkedList *cur = clouds;
		while (cur) {
			if (cur->next) {
				cur = cur->next;
			} else {
				break;
			}
		}
		if (cur->data) {
			return addToMass(cur->data, x, y);
		}
	}
	return NULL;
}

int breezeDir[2] = {-1, 1};

void breeze(void *) {
	for (linkedList *cur = clouds; cur != 0; cur = cur->next) {
		if (cur->data) {
			moveMass(cur->data, breezeDir[0], breezeDir[1]);
		}
	}
}

void rain(void *) {
	World *w = getWorld();
	//for (int x = 0; x < w->x; x++) {
		//for (int y = 0; y < w->y; y++) {
	for (linkedList *cur = clouds; cur; cur = cur->next) {
		Mass *clo = cur->data;
		for (linkedList *bod = clo->body; bod; bod = bod->next) {
			int *pos = bod->data;
			Cell *c = &w->map[(pos[1]*w->x)+pos[0]];
			bool covered = false;
			for (int i = FORMS_PER_CELL-1; i >= 0; i--) {
				if (c->within[i]) {
					if (getStat(c->within[i], COVER)) {
						covered = true;
						break;
					}
				}
			}
			if (!covered) {
				addEco(pos[0], pos[1], rainAmount);
			}

		}
	}
}

void *renderClouds(void *data) {
	Mass *cloud = data;
	RenderCommand reco = {
		.type = 3,
	};
	Tint tint = {
		.rgb = {
			0, 0, 0,
		},
		.value = -40,
		.strength = 0,
	};
	memcpy(reco.data, &tint, sizeof(Tint));
	renderMass(data, reco);
}

void freeClouds() {
	if (clouds) {
		linkedList *cur = clouds;
		deleteList(&clouds, freeMass);
		clouds = 0;
	}
}

