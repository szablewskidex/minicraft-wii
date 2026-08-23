#ifndef LEVEL_TILE_STONE_WALL_TILE_H_
#define LEVEL_TILE_STONE_WALL_TILE_H_

#include "tile.h"

struct _Player;
struct _Item;

void stonewalltile_init(TileID id);
void stonewalltile_render(TileID id, Screen* screen, Level* level, int x, int y);
void stonewalltile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir);
char stonewalltile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);

#endif /* LEVEL_TILE_STONE_WALL_TILE_H_ */
