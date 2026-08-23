#include "path_tile.h"
#include "../level.h"
#include "../../entity/player.h"
#include "../../entity/itementity.h"
#include "../../item/item.h"
#include "../../item/tool_item.h"
#include "../../item/resourceitem.h"
#include "../../item/resource/resource.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../sound.h"

void pathtile_init(TileID id) {
    tile_init(id);
    tiles[id].connectsToGrass = 1;
    tiles[id].connectsToSand = 1;
}

void pathtile_render(TileID id, Screen* screen, Level* level, int x, int y) {
    // 16x16 Earthy mossy dirt path at sprite offset x=12, y=42
    int col = getColor4(10, 210, 320, level->grassColor);

    render_screen(screen, (x * 16) + 0, (y * 16) + 0, (12 + 0) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 0, (12 + 1) + 42 * 32, col, 0);
    render_screen(screen, (x * 16) + 0, (y * 16) + 8, (12 + 0) + 43 * 32, col, 0);
    render_screen(screen, (x * 16) + 8, (y * 16) + 8, (12 + 1) + 43 * 32, col, 0);
}

void pathtile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir) {
    sound_play(SND_MONSTERHURT);
    level_set_tile(level, x, y, DIRT, 0);
}

char pathtile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir) {
    if (item && item->id == TOOL && item->add.tool.type == SHOVEL) {
        sound_play(SND_MONSTERHURT);
        level_set_tile(level, xt, yt, DIRT, 0);
        return 1;
    }
    return 0;
}
