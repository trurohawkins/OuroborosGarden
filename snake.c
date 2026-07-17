#include "snake.h"

int snakeCount = 0;

Snake *makeSnake(int xPos, int yPos) {
	Snake *s = calloc(1, sizeof(Snake));
	s->self = makeForm(SNAKE);

	SnakeBody *sb = makeBody(xPos, yPos);
	sb->roto = s->dir;
	s->body = makeList();
	addToList(&s->body, sb);

	addTimedEvent(snakeMovement, s, moveInterval);

	Player *player = checkPlayer(snakeCount+1);
	if (player == 0) {
		player = makePlayer(s, snakeCount+1, 0);
		addPlayer(player);
		if (snakeCount == 0) {
			addControl(player, "K0W", snakeUp);
			addControl(player, "K0A", snakeLeft);
			addControl(player, "K0S", snakeDown);
			addControl(player, "K0D", snakeRight);
			addControl(player, "K0P", snakeStep);
		}
	}
	placeSnake(s);
	for (int i = 0; i < baseSnake - 1;  i++ ) {
		growSnake(s);
	}

	addToList(&snakeList, s);
	snakeCount++;
	Nub *ren = growRenderNub(s->self);
	RenderObject *rob = ren->data;
	rob->data = s;
	rob->render = renderSnake;

	s->stamp = createStamp("@");
	/*
		 Sigil *skin = createSigil(s->self)->data;
		 skin->symbol = '@';
		 skin->figure = true;
		 skin->priority = 1;
		 skin->r = 255;
		 skin->g = 255;
		 skin->b = 255;
		 */
	return s;
}

SnakeBody *makeBody(int xp, int yp) {
	SnakeBody *sb = calloc(1, sizeof(SnakeBody));
	sb->pos[0] = xp;
	sb->pos[1] = yp;
	sb->roto = 1;
	sb->sprite = 0;
	return sb;
}

void growSnake(Snake *s) {
	int *tail = (int*)calloc(sizeof(int), 2);
	linkedList *cur = s->body;
	int head = true;
	while (cur != 0) {
		if (cur->next == 0) {
			if (cur->data != 0) {
				SnakeBody *sb = cur->data;
				memcpy(tail, sb->pos, sizeof(int) * 2);
				if (head) {
					sb->sprite = 0;
				} else {
					sb->sprite = 2;
				}
				break;
			} else {
				//printf("bad tail\n");
			}
		} else {
			head = false;
			cur = cur->next;
		}
	}
	int *dir = getDir4((s->dir+2) % 4);
	incPos(tail, tail + 1, dir[0], dir[1]);
	s->butt = makeBody(tail[0], tail[1]);
	s->butt->sprite = 1;
	s->butt->roto = (xyToDir4(dir) + 2) % 4;
	addToList(&s->body, s->butt);
	placeForm(s->self, tail[0], tail[1]);
	free(tail);
}

void placeSnake(Snake *s) {
	linkedList *cur = s->body;
	SnakeBody *head = cur->data;
	int overlap = 0;
	while (cur != 0) {
		if (cur->data != 0) {
			SnakeBody *sb = (SnakeBody*)cur->data;
			int x = sb->pos[0];
			int y = sb->pos[1];

			placeForm(s->self, x, y);
			addEco(x, y, ecoTrail);
			if (randPercent() < plantTrail) {
				placeGrass(x, y);
			}

			cur = cur->next;
		} else {
			//printf("bad body on snake %p\n", s);
		}
	}
}

void removeSnake(Snake *s) {
	linkedList *cur = s->body;
	while (cur != 0) {
		if (cur->data != 0) {
			//Pos *p = (Pos*)cur->data;
			SnakeBody *sb = (SnakeBody*)cur->data;
			removeForm(s->self, sb->pos[0], sb->pos[1]);
			cur = cur->next;
		} else {
			//printf("bad body on snake %p\n", s);
		}
	}
}

void turnSnake(Snake *s, int direction) {
	// if good direction
	//if (s->dir % 2 != direction % 2) {
	if ((s->dir + 2)%4 != direction ) {
		s->newDir = direction;
	}
}

bool snakeCheck(Snake *s) {
	int* d = getDir4(s->dir);
	SnakeBody *head = s->body->data;
	int posCheck[2] = {head->pos[0], head->pos[1]};
	incPos(posCheck, posCheck+1, d[0], d[1]);
	Cell *c = getCell(posCheck[0], posCheck[1]);
	Form *fruit = 0;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (c->within[i]) {
			Form *f = c->within[i];
			if (f->id == FLOWER) {
				float flowerGrowth = *getStat(f, STAGE);
				if (flowerGrowth >= 5) {
					fruit = f;
				}
			}
		}
	}
	if (fruit) {
		removeForm(fruit, fruit->pos[0], fruit->pos[1]);
		freeForm(fruit);
		growSnake(s);
	}
	return true;
}

bool moveSnake(Snake *s) {
	if (!snakeCheck(s)) {
		return false;
	}
	removeSnake(s);
	int pre[2] = {-5, -5};
	int dir[2];
	memcpy(dir, getDir4(s->dir), sizeof(int) * 2);

	linkedList *cur = s->body;
	while (cur) {
		// grab a reference to the current body part's position
		SnakeBody *sb = (SnakeBody*)cur->data;
		// if the direction doesn't match copy new direction for movement
		int d[2] = {pre[0] - sb->pos[0], pre[1] - sb->pos[1]};
		if (pre[0] != -5 && (dir[0] != d[0] || dir[1] != d[1])) {
			memcpy(dir, d, sizeof(int)*2);
		}
		//save current pos
		memcpy(pre, sb->pos, sizeof(int) * 2);
		//increment reference to pos
		incPos(sb->pos, sb->pos + 1, dir[0], dir[1]);
		cur = cur->next;
	}
	placeSnake(s);
	return true;
}

void snakeMovement(void *snake) {
	Snake *s = snake;
	if (s->staggered == 0) {
		int preDir = s->dir;
		s->dir = s->newDir;
		if (!moveSnake(s)) {
			s->dir = preDir;
			return;
		}
	}
}

void snakeStep(void *s, float val) {
	if (val == 1) {
		Snake *snake = s;
		snake->dir = snake->newDir;
		moveSnake(snake);
	}
}

void snakeUp(void *s, float val) {
	if (val == 1) {
		turnSnake(s, 0);
	}
}

void snakeLeft(void *s, float val) {
	if (val == 1) {
		turnSnake(s, 1);
	}
}


void snakeDown(void *s, float val) {
	if (val == 1) {
		turnSnake(s, 2);
	}
}

void snakeRight(void *s, float val) {
	if (val == 1) {
		turnSnake(s, 3);
	}
}


void freeSnake(void *s) {
	Snake *snake = s;

	removeSnake(snake);
	freeForm(snake->self);

	freeList(&snake->body);
	free(snake);
}

void *renderSnake(void *data) {
	Snake *s = data;
	linkedList *commands = 0;
	linkedList *body = s->body;
	RenderCommand reco = {
		.sigil = s->stamp,
		.r = 255,
		.g = 255,
		.b = 255,
		.layer = SNAKELAYER,
	};
	while (body) {
		SnakeBody *sb = body->data;
		reco.screenPos[0] = worldXToScreenX(sb->pos[0]);
		reco.screenPos[1] = worldYToScreenY(sb->pos[1]);
		addRenderCommand(reco);
		body = body->next;
	}
	return commands;
}
