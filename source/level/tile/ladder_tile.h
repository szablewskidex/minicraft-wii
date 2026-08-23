#ifndef LEVEL_TILE_LADDER_TILE_H_
#define LEVEL_TILE_LADDER_TILE_H_

#include "tile.h"
#include "../../gfx/screen.h"
#include "../level.h"

void laddertile_init(TileID id);
void laddertile_render(TileID id, Screen* screen, Level* level, int x, int y);
char laddertile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);
void laddertile_hurt(TileID id, Level* level, int x, int y, struct _Mob* source, int dmg, int attackDir);

#endif /* LEVEL_TILE_LADDER_TILE_H_ */
