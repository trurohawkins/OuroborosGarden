// world
int worldX = 40;
int worldY = 40;

const float spd = 0.3;
float spreadInterval = 4.0 * spd;
float moveInterval = 1.0 * spd;
float plantsInterval = 4.0 * spd;
float evapInterval = 8.0 * spd;
float rainInterval = 16.0 * spd;

// soil
int maxEco = 1;
float evaporation = 0.05;
float evapMinimum = 0.05;
float rainAmount = 0.05;
float spreadDither = 0.005;
float bioEvap = 0.1;
// snake
int baseSnake = 5;
int deadSnake = 2;
int fullStomach = 3;
int deathEco = 5;
float ecoTrail = 1.0f;
float plantTrail = 0.03;
//plants
float grassChance = 0;//.65f;
float flowerChance = 0.02;//.02f;

//Form IDs
#define DIRT 0
#define PLANT 1
#define SNAKE 2
#define GRASS 3
#define POOP 4
#define FLOWER 5
#define WATER 6
#define STONE 7
#define RAINBOW 8

//stat IDs
// dirt
#define ECO 0
#define OUTPUT 1
#define SOURCE 2
// plants
#define GROWTH 3
#define PULL 4
#define LOSS 5
#define ROOTS 6
#define COVER 7
#define BLOCK 8

//sprite layering
#define SNAKELAYER 5
#define FLOWERLAYER 4
#define POOPLAYER 3
#define RAINBOWLAYER 2
#define GRASSLAYER 1
#define DIRTLAYER 0

//NUBS
#define STATNUB 3
#define PLANTNUB 4

