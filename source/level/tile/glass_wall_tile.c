#include "glass_wall_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void glasswalltile_init(TileID id) {
    tile_init(id);
}

void glasswalltile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    TileID baseTile = (TileID)level_get_data(level, x, y);
    if (baseTile == 0) baseTile = (level->depth < 0) ? DIRT : GRASS;
    tile_render(baseTile, screen, level, x, y);

    int col = getColor4(-1, 100, 321, 555);

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (30 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (30 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (30 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (30 + 1) + 43 * 32, col, 0);
}

void glasswalltile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    sound_play(SND_EXPLODE);
    SmashParticle* sp = malloc(sizeof(SmashParticle));
    if (sp) {
        smashparticle_create(sp, (x * 16) + 8, (y * 16) + 8);
        level_addEntity(level, &sp->entity);
    }

    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &glassWallItem);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    TileID baseTile = (TileID)level_get_data(level, x, y);
    if (baseTile == 0) baseTile = (level->depth < 0) ? DIRT : GRASS;
    level_set_tile(level, x, y, baseTile, 0);
}

char glasswalltile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL) {
        glasswalltile_hurt(id, level, xt, yt, (Mob*)player, 10, attackDir);
        return 1;
    }
    return 0;
}
