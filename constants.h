// world
int worldX = 80;
int worldY = 40;

const float spd = 1;
float spreadInterval = 0.5 * spd;
int evapInterval = 3 * spd;
float moveInterval = 0.7 * spd;
float plantsInterval = 1.0 * spd;

// soil
int maxEco = 1;
float evaporation = 0.05;
float spreadDither = 0.005;
float bioEvap = 0.1;
// snake
int baseSnake = 6;
float ecoTrail = 0.05f;
float plantTrail = 0.03;
//plants

//Form IDs
#define DIRT 0
#define PLANT 1
#define SNAKE 2
#define GRASS 3
#define FLOWER 4

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

//sprite layering
#define SNAKELAYER 3
#define FLOWERLAYER 2
#define GRASSLAYER 1
#define DIRTLAYER 0

//NUBS
#define STATNUB 3
#define PLANTNUB 4

