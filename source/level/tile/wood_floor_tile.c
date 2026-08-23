#include "wood_floor_tile.h"
#include "../../gfx/color.h"
#include "../../item/item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../entity/itementity.h"
#include "../../sound.h"
#include "../../inputhandler.h"
#include <stdlib.h>

void woodfloortile_init(TileID id) {
	tile_init(id);
}

void woodfloortile_render(TileID id, Screen* screen, Level* level, int x, int y) {
	(void)id;
	(void)level;
	int col = getColor4(-1, 100, 321, 431);
	render_screen(screen, x * 16 + 0, y * 16 + 0, 0 + 42 * 32, col, 0);
	render_screen(screen, x * 16 + 8, y * 16 + 0, 1 + 42 * 32, col, 0);
	render_screen(screen, x * 16 + 0, y * 16 + 8, 0 + 43 * 32, col, 0);
	render_screen(screen, x * 16 + 8, y * 16 + 8, 1 + 43 * 32, col, 0);
}

char woodfloortile_interact(TileID id, Level* level, int xt, int yt, Player* player, Item* item, int attackDir) {
	(void)id;
	(void)attackDir;
	if (item->id == TOOL && item->add.tool.type == SHOVEL) {
		if (player_payStamina(player, 4 - item->add.tool.level)) {
			level_set_tile(level, xt, yt, DIRT, 0);
			sound_play(SND_MONSTERHURT);
			input_rumble(5);

			ItemEntity* entity = malloc(sizeof(ItemEntity));
			if (entity) {
				Item drop;
				resourceitem_create(&drop, &woodPlank);
				itementity_create(entity, drop, xt * 16 + 8, yt * 16 + 8);
				level_addEntity(level, (Entity*)entity);
			}
			return 1;
		}
	}
	return 0;
}
