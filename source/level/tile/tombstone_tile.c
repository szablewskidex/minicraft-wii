#include "tombstone_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../entity/skeleton.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void tombstonetile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
}

void tombstonetile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    tile_render((level->depth < 0) ? DIRT : GRASS, screen, level, x, y);

    int col = getColor4(-1, 111, 333, 555); // Aged stone cross/grave

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (20 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (20 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (20 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (20 + 1) + 43 * 32, col, 0);
}

void tombstonetile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    sound_play(SND_MONSTERHURT);

    // 40% chance of waking an angry Skeleton guardian!
    if (random_next_int(&level->random, 10) < 4) {
        Skeleton* skel = malloc(sizeof(Skeleton));
        if (skel) {
            skeleton_create(skel, 1);
            skel->mob.entity.x = (x * 16) + 8;
            skel->mob.entity.y = (y * 16) + 8;
            level_addEntity(level, &skel->mob.entity);
        }
    } else {
        // Treasure dropped from grave (Gold/Iron/Gem/Bones)
        Item item;
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        resourceitem_create_cnt(&item, (random_next_int(&level->random, 2) == 0) ? &goldIngot : &stone, 2);
        itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
        level_addEntity(level, &item_entity->entity);
    }

    level_set_tile(level, x, y, DIRT, 0);
}

char tombstonetile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && (item->add.tool.type == SHOVEL || item->add.tool.type == PICKAXE)) {
        tombstonetile_hurt(id, level, xt, yt, (Mob*)player, 10, attackDir);
        return 1;
    }
    return 0;
}
