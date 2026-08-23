#include "pot_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../item/item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void pottile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void pottile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    tile_render((level->depth < 0) ? DIRT : GRASS, screen, level, x, y);

    int col = getColor4(-1, 210, 420, 542); // Terracotta clay pot colors

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (18 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (18 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (18 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (18 + 1) + 43 * 32, col, 0);
}

void pottile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    // Smash particle & smash sound
    sound_play(SND_EXPLODE);
    SmashParticle* sp = malloc(sizeof(SmashParticle));
    if (sp) {
        smashparticle_create(sp, (x * 16) + 8, (y * 16) + 8);
        level_addEntity(level, &sp->entity);
    }

    // Zelda-like loot drop: arrows, iron, bread, apples, coal, or seeds!
    int lootPick = random_next_int(&level->random, 6);
    Resource* lootRes = &arrow;
    int count = 2;

    if (lootPick == 0) { lootRes = &arrow; count = 3; }
    else if (lootPick == 1) { lootRes = &ironOre; count = 1; }
    else if (lootPick == 2) { lootRes = &apple; count = 1; }
    else if (lootPick == 3) { lootRes = &coal; count = 2; }
    else if (lootPick == 4) { lootRes = &bread; count = 1; }
    else { lootRes = &seeds; count = 3; }

    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create_cnt(&item, lootRes, count);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    TileID baseTile = (level->depth < 0) ? DIRT : GRASS;
    level_set_tile(level, x, y, baseTile, 0);
}

char pottile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    pottile_hurt(id, level, xt, yt, (Mob*)player, 1, attackDir);
    return 1;
}
