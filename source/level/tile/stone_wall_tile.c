#include "stone_wall_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void stonewalltile_init(TileID id) {
    tile_init(id);
}

void stonewalltile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    int col = getColor4(-1, 111, 333, 555); // Solid stone brick colors

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (16 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (16 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (16 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (16 + 1) + 43 * 32, col, 0);
}

void stonewalltile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    int damage = level_get_data(level, x, y) + dmg;
    if (damage >= 25) { // High stone durability!
        Item item;
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        resourceitem_create(&item, &stoneWallItem);
        itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
        level_addEntity(level, &item_entity->entity);

        sound_play(SND_MONSTERHURT);
        level_set_tile(level, x, y, DIRT, 0);
    } else {
        level_set_data(level, x, y, damage);
    }
}

char stonewalltile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && item->add.tool.type == PICKAXE) {
        stonewalltile_hurt(id, level, xt, yt, (Mob*)player, (item->add.tool.level + 1) * 6, attackDir);
        return 1;
    }
    return 0;
}
