#ifndef LEVEL_TILE_STONE_FLOOR_TILE_H_
#define LEVEL_TILE_STONE_FLOOR_TILE_H_

#include "tile.h"
#include "../../gfx/screen.h"
#include "../level.h"

void stonefloortile_init(TileID id);
void stonefloortile_render(TileID id, Screen* screen, Level* level, int x, int y);
char stonefloortile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);
void stonefloortile_hurt(TileID id, Level* level, int x, int y, struct _Mob* source, int dmg, int attackDir);

#endif /* LEVEL_TILE_STONE_FLOOR_TILE_H_ */
