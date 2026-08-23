#include "wood_wall_tile.h"
#include "../../gfx/color.h"
#include "../../item/item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"
#include "../../sound.h"
#include "../../inputhandler.h"
#include <stdlib.h>
#include <stdio.h>

void woodwalltile_init(TileID id) {
	tile_init(id);
}

void woodwalltile_render(TileID id, Screen* screen, Level* level, int x, int y) {
	(void)id;
	(void)level;
	int col = getColor4(-1, 100, 321, 431);
	render_screen(screen, x * 16 + 0, y * 16 + 0, 2 + 42 * 32, col, 0);
	render_screen(screen, x * 16 + 8, y * 16 + 0, 3 + 42 * 32, col, 0);
	render_screen(screen, x * 16 + 0, y * 16 + 8, 2 + 43 * 32, col, 0);
	render_screen(screen, x * 16 + 8, y * 16 + 8, 3 + 43 * 32, col, 0);
}

void woodwalltile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
	(void)id;
	(void)source;
	(void)attackDir;
	sound_play(SND_MONSTERHURT);
	input_rumble(6);

	int damage = level_get_data(level, x, y) + dmg;

	SmashParticle* smash = malloc(sizeof(SmashParticle));
	if (smash) {
		smashparticle_create(smash, x * 16 + 8, y * 16 + 8);
		level_addEntity(level, (Entity*)smash);
	}

	TextParticle* text = malloc(sizeof(TextParticle));
	if (text) {
		char buf[16];
		sprintf(buf, "%d", dmg);
		textparticle_create(text, strdup(buf), x * 16 + 8, y * 16 + 8, getColor4(-1, 500, 500, 500));
		level_addEntity(level, (Entity*)text);
	}

	if (damage >= 20) {
		level_set_tile(level, x, y, WOOD_FLOOR, 0);

		ItemEntity* entity = malloc(sizeof(ItemEntity));
		if (entity) {
			Item drop;
			resourceitem_create(&drop, &woodWallItem);
			itementity_create(entity, drop, x * 16 + 8, y * 16 + 8);
			level_addEntity(level, (Entity*)entity);
		}
	} else {
		level_set_data(level, x, y, damage);
	}
}

char woodwalltile_interact(TileID id, Level* level, int xt, int yt, Player* player, Item* item, int attackDir) {
	if (item->id == TOOL) {
		if (item->add.tool.type == AXE || item->add.tool.type == PICKAXE) {
			if (player_payStamina(player, 4 - item->add.tool.level)) {
				int dmg = (item->add.tool.level + 1) * 5 + random_next_int(&tiles[id].random, 5);
				woodwalltile_hurt(id, level, xt, yt, (Mob*)player, dmg, attackDir);
				return 1;
			}
		}
	}
	return 0;
}
