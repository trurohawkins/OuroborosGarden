// world
int worldX = 80;
int worldY = 40;

// soil
int maxEco = 1;
float evaporation = 0.05;
float spreadDither = 0.005;
int spreadInterval = 6;
int evapInterval = 3;
float bioEvap = 0.1;
// snake
int baseSnake = 6;
float ecoTrail = 0.05f;
float plantTrail = 0.3;
float moveInterval = 0.7;
//plants
float plantsInterval = 1.0;

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
// plants
#define GROWTH 2
#define PULL 3
#define LOSS 4
#define ROOTS 5
#define COVER 6

//sprite layering
#define SNAKELAYER 3
#define FLOWERLAYER 2
#define GRASSLAYER 1
#define DIRTLAYER 0

//NUBS
#define STATNUB 3
#define PLANTNUB 4

