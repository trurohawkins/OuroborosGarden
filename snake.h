#pragma once
typedef struct SnakeBody {
	int pos[2];
	int roto;
	int sprite;
} SnakeBody;

typedef struct Snake {
	// holds array of 2 ints, cartesian coordinates
	linkedList *body;
	SnakeBody *butt;
	Form *self;
	int dir;
	int newDir;
} Snake;

Snake *makeSnake(int xPos, int yPos);
SnakeBody *makeBody(int xp, int yp);

void placeSnake(Snake *s);
void removeSnake(Snake *s);

void freeSnake(void *s);
