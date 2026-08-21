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

void game_reset();
void game_set_menu(enum menu_id menu);
void game_won();

void game_changeLevel(int dir);
