#include "wildflower_tile.h"
#include "../level.h"
#include "../../entity/itementity.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void wildflowertile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
}

void wildflowertile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    // 16x16 Wildflower patch at sprite offset x=10, y=42
    int col = getColor4(10, 30, 440, 550); // Dark foliage, green stem, warm orange & yellow blossoms

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (10 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (10 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (10 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (10 + 1) + 43 * 32, col, 0);
}

void wildflowertile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    Item item;
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    resourceitem_create(&item, &flower);
    itementity_create(item_entity, item, (x * 16) + 8, (y * 16) + 8);
    level_addEntity(level, &item_entity->entity);

    sound_play(SND_MONSTERHURT);
    level_set_tile(level, x, y, GRASS, 0);
}
