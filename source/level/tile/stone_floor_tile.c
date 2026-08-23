#include "stone_floor_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void stonefloortile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void stonefloortile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    int col = getColor4(111, 222, 333, 444);

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (28 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (28 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (28 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (28 + 1) + 43 * 32, col, 0);
}

void stonefloortile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &stoneFloor);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    sound_play(SND_MONSTERHURT);
    level_set_tile(level, x, y, (level->depth < 0) ? DIRT : GRASS, 0);
}

char stonefloortile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && (item->add.tool.type == PICKAXE || item->add.tool.type == SHOVEL)) {
        stonefloortile_hurt(id, level, xt, yt, (Mob*)player, 10, attackDir);
        return 1;
    }
    return 0;
}
