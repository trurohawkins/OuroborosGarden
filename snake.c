#include "snake.h"

int snakeCount = 0;
int staggerTime = 20;
float fullBodyEco = 20;

// head: 0 - 3
// butt: 4 - 7
// body straight: 8 - 11
// body bent0: 12 - 15
// body bent1: 16 - 19
int snakeStamps[20] = {-1};

Snake *makeSnake(int xPos, int yPos) {
	Snake *s = calloc(1, sizeof(Snake));
	s->self = makeForm(SNAKE);

	SnakeBody *sb = makeBody(xPos, yPos);
	sb->roto = s->dir;
	s->body = makeList();

	addToList(&s->body, sb);

	s->eNum = addTimedEvent(snakeAction, s, moveInterval);
	s->stomach = 0;//fullStomach;
	s->pooInterval = 100;
	s->pooLength = 10;
	s->pNum = snakeCount;
	Player *player = checkPlayer(snakeCount+1);
	if (player == 0) {
		player = makePlayer(s, snakeCount+1, 0);
		addPlayer(player);
		if (snakeCount == 0) {
			addKeyControl(player, 'W', snakeUp);
			addKeyControl(player, 'A', snakeLeft);
			addKeyControl(player, 'S', snakeDown);
			addKeyControl(player, 'D', snakeRight);
			addKeyControl(player, 'P', snakeStep);
			addKeyControl(player, 129, snakeUp);
			addKeyControl(player, 130, snakeLeft);
			addKeyControl(player, 131, snakeDown);
			addKeyControl(player, 132, snakeRight);
		}
	} else {
		//probably on a restart and need to update snake
		player->self = s;
	}
	placeSnake(s);
	/*
	for (int i = 0; i < baseSnake - 1;  i++ ) {
		growSnake(s);
	}
	*/

	addToList(&snakeList, s);
	snakeCount++;
	Nub *ren = growRenderNub(s->self);
	RenderObject *rob = ren->data;
	rob->data = s;
	rob->render = renderSnake;
	
	if (snakeStamps[0] == -1) {
		bool braille = false;
		//braille
		if (braille) {
			snakeStamps[0] = createStamp("\u28FE", "\u28F7");
			snakeStamps[1] = createStamp("\u28BE", "\u28FF");
			snakeStamps[2] = createStamp("\u28BF", "\u287F");
			snakeStamps[3] = createStamp("\u28FF", "\u2877");
			

			snakeStamps[4] = createStamp("\u28BF", "\u287F");
			snakeStamps[5] = createStamp("\u28FF", "\u2877");
			snakeStamps[6] = createStamp("\u28FE", "\u28F7");
			snakeStamps[7] = createStamp("\u28BE", "\u28FF");
			//snakeStamps[4] = createStamp("\u28BE", "\u2877");
			snakeStamps[8] = createStamp("\u28FF", "\u28FF");
			snakeStamps[9] = createStamp("\u28FF", "\u28FF");
			snakeStamps[10] = createStamp("\u28FF", "\u28FF");
			snakeStamps[11] = createStamp("\u28FF", "\u28FF");
			//snakeStamps[4] = createStamp("\u28B8", "\u2847");

			snakeStamps[12] = createStamp("\u28FF", "\u28FF");
			snakeStamps[13] = createStamp("\u28FF", "\u28FF");
			snakeStamps[14] = createStamp("\u28FF", "\u28FF");
			snakeStamps[15] = createStamp("\u28FF", "\u28FF");

			snakeStamps[16] = createStamp("\u28FF", "\u28FF");
			snakeStamps[17] = createStamp("\u28FF", "\u28FF");
			snakeStamps[18] = createStamp("\u28FF", "\u28FF");
			snakeStamps[19] = createStamp("\u28FF", "\u28FF");
		} else {
			/*
			char *head = "\u2588";
			snakeStamps[0] = createStamp(head, head);
			snakeStamps[1] = createStamp(head, head);
			snakeStamps[2] = createStamp(head, head);
			snakeStamps[3] = createStamp(head, head);
			snakeStamps[0] = createStamp("\u2588", "\u2584");
			snakeStamps[1] = createStamp("\u2584", "\u2588");
			snakeStamps[2] = createStamp("\u2580", "\u2588");
			snakeStamps[3] = createStamp("\u2588", "\u2580");
			*/
			char *normFace = "\U0001F784";
			snakeStamps[0] = createStamp(normFace, normFace);
			char *xEyes = "\U0001F7AE";
			snakeStamps[1] = createStamp(xEyes, xEyes);
			char *stunEyes = "\U0001F7B7";
			snakeStamps[2] = createStamp(stunEyes, stunEyes);
			//char *pooEyes = "\u18D5";
			//char *pooEyes = "\u05D7";
			//char *pooEyes = "\u2040";
			//char *pooEyes = "\u2E1A";
			//snakeStamps[0] = createStamp(pooEyes, 0);
			char *pooEyes = "\u2322";
			snakeStamps[3] = createStamp(pooEyes, pooEyes);
			//snakeStamps[0] = createStamp(">", "<");
			//snakeStamps[0] = createStamp("n", "n");
			
			char *bod0 = "\u259A";
			char *butt = bod0;//"\u2588"; 
			snakeStamps[4] = createStamp(butt, butt);
			snakeStamps[5] = createStamp(butt, butt);
			snakeStamps[6] = createStamp(butt, butt);
			snakeStamps[7] = createStamp(butt, butt);

			snakeStamps[8] = createStamp(bod0, bod0);
			snakeStamps[9] = createStamp(bod0, bod0);
			snakeStamps[10] = createStamp(bod0, bod0);
			snakeStamps[11] = createStamp(bod0, bod0);

			snakeStamps[12] = createStamp(bod0, bod0);
			snakeStamps[13] = createStamp(bod0, bod0);
			snakeStamps[14] = createStamp(bod0, bod0);
			snakeStamps[15] = createStamp(bod0, bod0);
			
			snakeStamps[16] = createStamp(bod0, bod0);
			snakeStamps[17] = createStamp(bod0, bod0);
			snakeStamps[18] = createStamp(bod0, bod0);
			snakeStamps[19] = createStamp(bod0, bod0);
		}
	}
	return s;
}

SnakeBody *makeBody(int xp, int yp) {
	SnakeBody *sb = calloc(1, sizeof(SnakeBody));
	sb->pos[0] = xp;
	sb->pos[1] = yp;
	sb->roto = 1;
	sb->sprite = 0;
	sb->eco = fullBodyEco;
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
					sb->sprite = 8;
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
	s->butt->sprite = 4;
	s->butt->roto = (xyToDir4(dir) + 2) % 4;
	addToList(&s->body, s->butt);
	placeForm(s->self, tail[0], tail[1]);
	free(tail);
}

void shrinkSnake(Snake *s) {
	linkedList *cur = s->body;
	SnakeBody *tail = 0;
	int roto = -1;
	while (cur != 0) {
		SnakeBody *sb = cur->data;
		// we dotn wanna shrink passed 2 pieces, head and butt
		if (cur->data != s->body->data) {
			if (cur->next && cur->next->next == 0) {
				tail = cur->next->data;
				free(cur->next);
				cur->next = 0;
				s->butt = cur->data;
				s->butt->roto = roto;
				s->butt->sprite = 4;
			}
		}
		roto = sb->roto;
		cur = cur->next;
	}
	if (tail) {
		removeForm(s->self, tail->pos[0], tail->pos[1]);
		free(tail);
	}
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
			if (checkFormID(x, y, WATER)) {
				if (sb->eco + 1 < fullBodyEco) {
					sb->eco += 1;
				} else {
					sb->eco = fullBodyEco;
				}
			}
			Form *soil = checkSoil(x, y);
			if (soil) {
				if (sb->eco - ecoTrail > 0) {
					sb->eco -= addEco(x, y, ecoTrail);
				} else {
					addEco(x, y, sb->eco);
					sb->eco = 0;
				}
				if (randPercent() < plantTrail) {
					placeGrass(x, y);
				}
			}
			placeForm(s->self, x, y);
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
	Form *crush = 0;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (c->within[i]) {
			Form *f = c->within[i];
			if (f->id == FLOWER) {
				Plant *p = getPlant(f);
				if (p->stage > 2) {
					fruit = f;
				} else if (p->stage > 0) {
					crush = f;
				}
			} else if (f->id == STONE) {
				snakeStagger(s, true);
				return false;
			}
		}
	}
	if (fruit) {
		removeForm(fruit, fruit->pos[0], fruit->pos[1]);
		freeForm(fruit);
		growSnake(s);
		/*
		if (s->stomach < fullStomach) {
			s->stomach++;
		}
		*/
	}
	if (crush != 0) {
		if (crush->id == FLOWER) {
			plantDie(crush);
		}
	}
	return true;
}

bool moveSnake(Snake *s) {
	if (!snakeCheck(s)) {
		return false;
	}
	if (!s->grown) {
		if (countSnakeParts(s) < baseSnake) {
			growSnake(s);
		} else {
			s->grown = true;
		}
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
		if (pre[0] == -5) {
			sb->roto = s->dir;
		} else if (abs(d[0]) > 1 || abs(d[1]) > 1) {
			d[0] = sign(d[0]) * -1;
			d[1] = sign(d[1]) * -1;
			sb->roto = xyToDir4(d);
		} else {
			sb->roto = xyToDir4(d);
		}
		if (pre[0] != -5 && (dir[0] != d[0] || dir[1] != d[1])) {
			if (sb->sprite >= 8) {
				int preD = xyToDir4(dir);
				int newD = xyToDir4(d);
				if ((preD + 1) % 4 == newD) {
					sb->sprite = 12;
				} else {
					sb->sprite = 16;
				}
			} else if (sb->sprite == 4) {
				sb->roto = xyToDir4(dir);
			}
			memcpy(dir, d, sizeof(int)*2);
		} else {
			if (sb->sprite > 8) {
				sb->sprite = 8;
			}
		}
		//save current pos
		memcpy(pre, sb->pos, sizeof(int) * 2);
		//increment reference to pos
		incPos(sb->pos, sb->pos + 1, dir[0], dir[1]);
		cur = cur->next;
	}
	placeSnake(s);
	ouroboros(s);
	s->moving = true;
	return true;
}

void snakeAction(void *snake) {
	Snake *s = snake;
	if (s->pooCounter >= s->pooInterval) {
		s->pooCounter = -s->pooLength;
		if (countSnakeParts(s) > deadSnake) {
			if (s->staggered == 0) {
				s->state = 3;
			}
		} else {
			s->state = 1;
		}
	} else {
		s->pooCounter++;
	}

	if (s->staggered == 0 && s->pooCounter >= 0) {
		int preDir = s->dir;
		s->dir = s->newDir;
		if (!moveSnake(s)) {
			s->dir = preDir;
			return;
		}
		checkAndDelete(&s->rainbows, checkRainbow, freeRainbow);
	} else  {
		if (s->staggered > 0) {
			s->staggered--;
			if (s->staggered == 0) {
				snakeStagger(s, false);
			}
		}
		if (s->pooCounter == -1) {
			if (s->staggered == 0) {
				s->state = 0;
			} else {
				s->state = 2;
			}
			snakePoop(s);
		}
	}
}

void snakeStagger(Snake *s, bool staggered) {
	if (staggered) {
		if (s->moving) {
			s->staggered = staggerTime;
			s->state = 2;
			s->moving = false;
		}
	} else {
		s->state = 0;
	}
}

void snakePoop(Snake *s) {
	//if (s->stomach > 0) {
		//s->stomach--;
	//} else {
		if (countSnakeParts(s) > deadSnake) {
			int pos[2] = {s->butt->pos[0], s->butt->pos[1]};
			Form *p = makePoop();
			placeForm(p, pos[0], pos[1]);
			shrinkSnake(s);
		} else {
			snakeDie(s);
		}
	//}
}


void ouroboros(Snake *s) {
	SnakeBody *head = s->body->data;
	SnakeBody *neck = s->body->next->data;
	spaceCheck(s, head->pos[0], head->pos[1]);
	spaceCheck(s, neck->pos[0], neck->pos[1]);
	spaceCheck(s, s->butt->pos[0], s->butt->pos[1]);
}

void spaceCheck(Snake *s, int x, int y) {
	for (int i = 0; i < 4; i++) {
		int *d = getDir4((s->dir+i)%4);
		Rainbow *r = fill(x + d[0], y + d[1]);
		if (r != 0) {
			addToList(&s->rainbows, r);
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

int countSnakeParts(Snake *s) {
	linkedList *cur = s->body;
	int count = 0;
	while (cur != 0) {
		count++;
		cur = cur->next;
	}
	return count;
}

float percentSnakeEco(Snake *s) {
	linkedList *cur = s->body;
	float eco = 0;
	int count = 0;
	while (cur != 0) {
		SnakeBody *sb = cur->data;
		eco += sb->eco / fullBodyEco;
		count++;
		cur = cur->next;
	}
	return eco / count;
}

void snakeDie(Snake *s) {
	linkedList *cur = s->body;
	while (cur) {
		SnakeBody *sb = cur->data;
		pushEco(sb->pos[0], sb->pos[1], deathEco);
		cur = cur->next;
	}
	removeFromList(&snakeList, s);
	freeSnake(s);
}


void freeSnake(void *s) {
	Snake *snake = s;
	Player *p = checkPlayer(snake->pNum + 1);
	removePlayer(p);
	freePlayer(p);
	//remvoe audio movement
	unscheduleEvent(snake->eNum);

	removeSnake(snake);
	freeForm(snake->self);
	deleteList(&snake->rainbows, freeRainbow);

	freeList(&snake->body);
	free(snake);
}

float snakeHealth[3] = {255, 255, 255};
float snakeDeath[3] = {150, 150, 150};

void *renderSnake(void *data) {
	Snake *s = data;
	linkedList *commands = 0;
	linkedList *body = s->body;
	RenderCommand reco = {
		.type = 0,
		.layer = SNAKELAYER+1,
	};
	float eco = percentSnakeEco(s);//(float)s->stomach / (float)fullStomach;
	Color col = {
		.rgb = {
			lerp(snakeDeath[0], snakeHealth[0], eco),
			lerp(snakeDeath[1], snakeHealth[1], eco),
			lerp(snakeDeath[2], snakeHealth[2], eco),
		},
	};
	memcpy(reco.data, &col, sizeof(Color));
	while (body) {
		SnakeBody *sb = body->data;
		//float eco = sb->eco / fullBodyEco;
		reco.pos.x = worldXToScreenX(sb->pos[0]);
		reco.pos.y = worldYToScreenY(sb->pos[1]);
		//snake head
		if (body->data == s->body->data) {
			reco.index = -1;//snakeStamps[sb->sprite+sb->roto];
			addRenderCommand(reco);
			for (int i = 0; i < 3; i++) {
				col.rgb[i] = 0;
			}
			memcpy(reco.data, &col, sizeof(Color));
			reco.index = snakeStamps[s->state];
			addRenderCommand(reco);
			reco.layer = SNAKELAYER;
			for (int i = 0; i < 3; i++) {
				col.rgb[i] = lerp(snakeDeath[0], snakeHealth[0], eco);
			}
			memcpy(reco.data, &col, sizeof(Color));
		} else {
			reco.index = snakeStamps[sb->sprite+sb->roto];
			addRenderCommand(reco);
		}
		body = body->next;
	}
	return commands;
}
