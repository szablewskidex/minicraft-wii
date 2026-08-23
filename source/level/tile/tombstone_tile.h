#ifndef LEVEL_TILE_TOMBSTONE_TILE_H_
#define LEVEL_TILE_TOMBSTONE_TILE_H_

#include "tile.h"

struct _Player;
struct _Item;

void tombstonetile_init(TileID id);
void tombstonetile_render(TileID id, Screen* screen, Level* level, int x, int y);
void tombstonetile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir);
char tombstonetile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir);

#endif /* LEVEL_TILE_TOMBSTONE_TILE_H_ */
