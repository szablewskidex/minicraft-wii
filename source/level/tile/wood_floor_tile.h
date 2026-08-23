#ifndef LEVEL_TILE_WOOD_FLOOR_TILE_H_
#define LEVEL_TILE_WOOD_FLOOR_TILE_H_

#include "tile.h"

struct _Player;
struct _Item;

void woodfloortile_init(TileID id);
void woodfloortile_render(TileID id, Screen* screen, Level* level, int x, int y);
char woodfloortile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);

#endif /* LEVEL_TILE_WOOD_FLOOR_TILE_H_ */
