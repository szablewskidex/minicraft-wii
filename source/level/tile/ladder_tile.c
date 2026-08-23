#include "ladder_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void laddertile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void laddertile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    TileID baseTile = (TileID)level_get_data(level, x, y);
    if (baseTile == 0) baseTile = (level->depth < 0) ? DIRT : GRASS;
    tile_render(baseTile, screen, level, x, y);

    int col = getColor4(-1, 100, 321, 431); // Wooden rungs & rails

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (0 + 0) + 48 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (0 + 1) + 48 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (0 + 0) + 49 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (0 + 1) + 49 * 32, col, 0);
}

void laddertile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &ladderItem);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    sound_play(SND_MONSTERHURT);
    TileID baseTile = (TileID)level_get_data(level, x, y);
    if (baseTile == 0) baseTile = (level->depth < 0) ? DIRT : GRASS;
    level_set_tile(level, x, y, baseTile, 0);
}

char laddertile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && (item->add.tool.type == AXE || item->add.tool.type == PICKAXE)) {
        laddertile_hurt(id, level, xt, yt, (Mob*)player, 10, attackDir);
        return 1;
    }
    return 0;
}
