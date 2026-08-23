#include "tile.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"
#include "../../entity/itementity.h"
#include "../../item/resourceitem.h"
#include "../../gfx/color.h"

#include "../../sound.h"
#include "../../inputhandler.h"


void treetile_init(TileID id) {
	tile_init(id);
	tiles[id].connectsToGrass = 1;
}


void treetile_hurt2(TileID id, Level* level, int x, int y, int dmg) {
	sound_play(SND_MONSTERHURT);
	input_rumble(6);
	int count = random_next_int(&tiles[id].random, 10) == 0 ? 1 : 0;
	Random* random = &tiles[id].random;

	for (int i = 0; i < count; i++) {
        Item res;
		ItemEntity* ent = malloc(sizeof(ItemEntity));
		resourceitem_create(&res, &apple);

		int xx = (x * 16) + random_next_int(random, 10) + 3;
		int yy = (y * 16) + random_next_int(random, 10) + 3;

		itementity_create(ent, res, xx, yy);
		level_addEntity(level, &ent->entity);
	}

	int damage = level_get_data(level, x, y) + dmg;
	SmashParticle* smash = malloc(sizeof(SmashParticle));
	smashparticle_create(smash, (x * 16) + 8, (y * 16) + 8);
	level_addEntity(level, &smash->entity);

	TextParticle* text = malloc(sizeof(TextParticle));
	char* txt = malloc(16);
	sprintf(txt, "%d", dmg);
	textparticle_create(text, txt, (x * 16) + 8, (y * 16) + 8, getColor4(-1, 500, 500, 500));
	level_addEntity(level, &text->entity);

	if (damage >= 20) {
		int count = random_next_int(random, 2) + 1;

		for (int i = 0; i < count; ++i) {
            Item res;
			ItemEntity* ent = malloc(sizeof(ItemEntity));
			resourceitem_create(&res, &wood);

			int xx = (x * 16) + random_next_int(random, 10) + 3;
			int yy = (y * 16) + random_next_int(random, 10) + 3;

			itementity_create(ent, res, xx, yy);
			level_addEntity(level, &ent->entity);
		}

		count = random_next_int(random, random_next_int(random, 4) + 1);
		for (int i = 0; i < count; ++i) {
            Item res;
			ItemEntity* ent = malloc(sizeof(ItemEntity));
			resourceitem_create(&res, &acorn);

			int xx = (x * 16) + random_next_int(random, 10) + 3;
			int yy = (y * 16) + random_next_int(random, 10) + 3;

			itementity_create(ent, res, xx, yy);
			level_addEntity(level, &ent->entity);
		}

		level_set_tile(level, x, y, GRASS, 0);
	} else {
		level_set_data(level, x, y, damage);
	}
}


void treetile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
	treetile_hurt2(id, level, x, y, dmg);
}


#define IS_TREE(t) ((t) == TREE || (t) == BIRCH_TREE || (t) == SPRUCE_TREE)

void treetile_render(TileID id, Screen* screen, Level* level, int x, int y) {
	int col = getColor4(10, 30, 151, level->grassColor);
	int barkCol1 = getColor4(10, 30, 430, level->grassColor);
	int barkCol2 = getColor4(10, 30, 320, level->grassColor);

	if (id == BIRCH_TREE) {
		col = getColor4(10, 40, 252, level->grassColor);
		barkCol1 = getColor4(10, 40, 555, level->grassColor);
		barkCol2 = getColor4(10, 40, 444, level->grassColor);
	} else if (id == SPRUCE_TREE) {
		col = getColor4(10, 20, 131, level->grassColor);
		barkCol1 = getColor4(10, 20, 210, level->grassColor);
		barkCol2 = getColor4(10, 20, 100, level->grassColor);
	}

	char u = IS_TREE(level_get_tile(level, x, y - 1));
	char l = IS_TREE(level_get_tile(level, x - 1, y));
	char r = IS_TREE(level_get_tile(level, x + 1, y));
	char d = IS_TREE(level_get_tile(level, x, y + 1));

	char ul = IS_TREE(level_get_tile(level, x - 1, y - 1));
	char ur = IS_TREE(level_get_tile(level, x + 1, y - 1));
	char dl = IS_TREE(level_get_tile(level, x - 1, y + 1));
	char dr = IS_TREE(level_get_tile(level, x + 1, y + 1));

	if (u && ul && l) {
        render_screen(screen, (x * 16) + 0, (y * 16) + 0, 10 + 1 * 32, col, 0);
    } else {
        render_screen(screen, (x * 16) + 0, (y * 16) + 0, 9 + 0 * 32, col, 0);
    }

	if (u && ur && r) {
        render_screen(screen, (x * 16) + 8, (y * 16) + 0, 10 + 2 * 32, barkCol2, 0);
    } else {
        render_screen(screen, (x * 16) + 8, (y * 16) + 0, 10 + 0 * 32, col, 0);
    }

	if (d && dl && l) {
        render_screen(screen, (x * 16) + 0, (y * 16) + 8, 10 + 2 * 32, barkCol2, 0);
    } else {
        render_screen(screen, (x * 16) + 0, (y * 16) + 8, 9 + 1 * 32, barkCol1, 0);
    }

	if (d && dr && r) {
        render_screen(screen, (x * 16) + 8, (y * 16) + 8, 10 + 1 * 32, col, 0);
    } else {
        render_screen(screen, (x * 16) + 8, (y * 16) + 8, 10 + 3 * 32, barkCol2, 0);
    }
}


void treetile_tick(TileID id, Level* level, int xt, int yt) {
	int damage = level_get_data(level, xt, yt);
	if (damage) {
        level_set_data(level, xt, yt, damage - 1);
    }
}


char treetile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
	if (item->id == TOOL) {
		if (item->add.tool.type == AXE) {
			if (player_payStamina(player, 4 - item->add.tool.level)) {
				treetile_hurt2(id, level, xt, yt, random_next_int(&tiles[id].random, 10) + (item->add.tool.level * 5) + 10);
				return 1;
			}
		}
	}
	return 0;
}
