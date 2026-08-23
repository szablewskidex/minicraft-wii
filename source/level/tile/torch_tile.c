#include "torch_tile.h"
#include "../level.h"
#include "../../entity/player.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void torchtile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void torchtile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    tile_render((level->depth < 0) ? DIRT : GRASS, screen, level, x, y);

    int frame = ((tile_tickCount / 10) % 2 == 0) ? 6 : 8;
    int torchCol = getColor4(-1, 100, 520, 550);

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (frame + 0) + 42 * 32, torchCol, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (frame + 1) + 42 * 32, torchCol, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (frame + 0) + 43 * 32, torchCol, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (frame + 1) + 43 * 32, torchCol, 0);
}

void torchtile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &torchItem);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    sound_play(SND_MONSTERHURT);
    TileID baseTile = (level->depth < 0) ? DIRT : GRASS;
    level_set_tile(level, x, y, baseTile, 0);
}

char torchtile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    torchtile_hurt(id, level, xt, yt, (Mob*)player, 1, attackDir);
    return 1;
}
