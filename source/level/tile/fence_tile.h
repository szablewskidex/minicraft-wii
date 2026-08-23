#ifndef LEVEL_TILE_FENCE_TILE_H_
#define LEVEL_TILE_FENCE_TILE_H_

#include "tile.h"

struct _Player;
struct _Item;

void fencetile_init(TileID id);
void fencetile_render(TileID id, Screen* screen, Level* level, int x, int y);
void fencetile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir);
char fencetile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);

#endif /* LEVEL_TILE_FENCE_TILE_H_ */
