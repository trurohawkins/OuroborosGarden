#pragma once
typedef struct SnakeBody {
	int pos[2];
	int roto;
	int sprite;

	float eco;
} SnakeBody;

typedef struct Snake {
	// holds array of 2 ints, cartesian coordinates
	linkedList *body;
	SnakeBody *butt;
	Form *self;
	linkedList *rainbows;

	int dir;
	int newDir;
	
	int state;
	int staggered;
	bool moving;
	
	int stomach;
	int pooCounter;
	int pooInterval;
	int pooLength;
	bool grown;

	// for player event
	int pNum;
	//for audio event
	int eNum;
} Snake;

Snake *makeSnake(int xPos, int yPos);
SnakeBody *makeBody(int xp, int yp);

void growSnake(Snake *s);
void placeSnake(Snake *s);
void removeSnake(Snake *s);

void turnSnake(Snake *s, int direction);
bool snakeCheck(Snake *s);
bool moveSnake(Snake *s);
void snakeAction(void *s);
void snakeStagger(Snake *s, bool staggered);
void ouroboros(Snake *s);
void spaceCheck(Snake *s, int x, int y);
void snakePoop(Snake *s);

void snakeStep(void *s, float val);
void snakeUp(void *s, float val);
void snakeLeft(void *s, float val);
void snakeDown(void *s, float val);
void snakeRight(void *s, float val);

int countSnakeParts(Snake *s);
float percentSnakeEco(Snake *s);
void snakeDie(Snake *s);

void freeSnake(void *s);
void *renderSnake(void *data);

