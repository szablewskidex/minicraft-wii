#ifndef LEVEL_TILE_GLASS_WALL_TILE_H_
#define LEVEL_TILE_GLASS_WALL_TILE_H_

#include "tile.h"
#include "../../gfx/screen.h"
#include "../level.h"

void glasswalltile_init(TileID id);
void glasswalltile_render(TileID id, Screen* screen, Level* level, int x, int y);
char glasswalltile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);
void glasswalltile_hurt(TileID id, Level* level, int x, int y, struct _Mob* source, int dmg, int attackDir);

#endif /* LEVEL_TILE_GLASS_WALL_TILE_H_ */
