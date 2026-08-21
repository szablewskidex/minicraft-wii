#include "gfx/spritesheet.h"
#include "entity/player.h"
#include "screen/menu.h"

#ifdef __wii__
#define HEIGHT 240
#define WIDTH 320
#define SCALE 2
#else
#define HEIGHT 192
#define WIDTH 288
#define SCALE 3
#endif

extern Player* game_player;
extern int game_gameTime;
extern int game_pendingLevelChange;
extern char isingame;
extern Level game_levels[5];
extern Level* game_level;
extern int game_currentLevel;
extern char game_hasWon;
extern int game_wonTimer;

void game_reset();
void game_set_menu(enum menu_id menu);
void game_won();

void game_changeLevel(int dir);
