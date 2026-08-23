#include "door.h"
#include "../gfx/color.h"
#include "../sound.h"
#include "player.h"

void door_create(Door* door){
    furniture_create((Furniture*)door, "Door");
    door->isOpen = 0;
    door->furniture.col = getColor4(-1, 100, 321, 431); // Wooden door colors
    door->furniture.sprite = 5;
    door->furniture.entity.type = DOOR;
    door->furniture.entity.xr = 7;
    door->furniture.entity.yr = 7;
}

char door_use(Door* door, struct _Player* player, int attackDir){
    (void)player;
    (void)attackDir;
    door->isOpen = !door->isOpen;
    if (door->isOpen) {
        door->furniture.entity.xr = 0;
        door->furniture.entity.yr = 0;
    } else {
        door->furniture.entity.xr = 7;
        door->furniture.entity.yr = 7;
    }
    sound_play(SND_CONFIRM);
    return 1;
}

void door_render(Door* door, Screen* screen) {
    int x = door->furniture.entity.x;
    int y = door->furniture.entity.y;
    int col = door->furniture.col;
    int xt = door->isOpen ? 2 : 0;
    int yt = 46;

    render_screen(screen, x - 8, y - 8 - 4, xt + yt * 32, col, 0);
    render_screen(screen, x - 0, y - 8 - 4, xt + 1 + yt * 32, col, 0);
    render_screen(screen, x - 8, y - 0 - 4, xt + (yt + 1) * 32, col, 0);
    render_screen(screen, x - 0, y - 0 - 4, xt + 1 + (yt + 1) * 32, col, 0);
}
