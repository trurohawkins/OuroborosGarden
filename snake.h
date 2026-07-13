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

	int staggered;
} Snake;

Snake *makeSnake(int xPos, int yPos);
SnakeBody *makeBody(int xp, int yp);

void growSnake(Snake *s);
void placeSnake(Snake *s);
void removeSnake(Snake *s);

void turnSnake(Snake *s, int direction);
bool moveSnake(Snake *s);
void snakeMovement(void *s);

void snakeStep(void *s, float val);
void snakeUp(void *s, float val);
void snakeLeft(void *s, float val);
void snakeDown(void *s, float val);
void snakeRight(void *s, float val);


void freeSnake(void *s);
void incPos(int *px, int *py, int dx, int dy);
void *renderSnake(void *data);

