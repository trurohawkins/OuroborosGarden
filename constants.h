// world
int worldX = 80;
int worldY = 40;

// soil
int maxEco = 1;
float evaporation = 0.05;
float spreadDither = 0.01;
int spreadInterval = 6;
int evapInterval = 3;
float bioEvap = 0.1;
// snake
int baseSnake = 10;
float ecoTrail = 0.17f;
float plantTrail = 0.3;
float moveInterval = 1.0;
//plants
float plantsInterval = 2.0;

//Form IDs
#define DIRT 0
#define PLANT 1
#define SNAKE 2
#define GRASS 3

//stat IDs
// dirt
#define ECO 0
#define OUTPUT 1
// plants
#define GROWTH 2
#define PULL 3
#define LOSS 4
#define ROOTS 5
#define COVER 6
#define LIFE 7
#define BEAT 8
#define CYCLE 9
#define STAGE 10 
#define LIFETIME 11
