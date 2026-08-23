#include "fence_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void fencetile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void fencetile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    int bgCol = (level->depth < 0) ? getColor4(111, 222, 333, 222) : getColor4(level->grassColor, level->grassColor, level->grassColor + 111, level->grassColor + 111);
    render_screen(screen, (x * 16) + 0, (y * 16) + 0, 0, bgCol, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, 1, bgCol, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, 2, bgCol, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, 3, bgCol, 0);

    int col = getColor4(-1, 100, 321, 431); // Rustic wooden fence

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (22 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (22 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (22 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (22 + 1) + 43 * 32, col, 0);
}

void fencetile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &fenceItem);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    sound_play(SND_MONSTERHURT);
    TileID baseTile = (level->depth < 0) ? DIRT : GRASS;
    level_set_tile(level, x, y, baseTile, 0);
}

char fencetile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && item->add.tool.type == AXE) {
        fencetile_hurt(id, level, xt, yt, (Mob*)player, 10, attackDir);
        return 1;
    }
    return 0;
}
