#ifndef LEVEL_TILE_WILDFLOWER_TILE_H_
#define LEVEL_TILE_WILDFLOWER_TILE_H_

#include "tile.h"

void wildflowertile_init(TileID id);
void wildflowertile_render(TileID id, Screen* screen, Level* level, int x, int y);
void wildflowertile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir);

#endif /* LEVEL_TILE_WILDFLOWER_TILE_H_ */
