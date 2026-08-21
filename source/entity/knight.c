#include "knight.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void knight_create(Knight* knight, int lvl){
    mob_create(&knight->mob);
    knight->mob.entity.type = KNIGHT;
    knight->lvl = lvl;
    knight->mob.entity.x = random_next_int(&knight->mob.entity.random, 64 * 16);
    knight->mob.entity.y = random_next_int(&knight->mob.entity.random, 64 * 16);
    knight->mob.health = knight->mob.maxHealth = lvl * 25;
    knight->randomWalkTime = 0;
    knight->xa = knight->ya = 0;
}

void knight_tick(Knight* knight) {
    mob_tick(&knight->mob);
    Random* random = &knight->mob.entity.random;

    if (game_player && game_player->mob.entity.level == knight->mob.entity.level) {
        int xd = game_player->mob.entity.x - knight->mob.entity.x;
        int yd = game_player->mob.entity.y - knight->mob.entity.y;

        if (((xd * xd) + (yd * yd)) < (60 * 60)) {
            knight->xa = (xd < 0) ? -1 : ((xd > 0) ? 1 : 0);
            knight->ya = (yd < 0) ? -1 : ((yd > 0) ? 1 : 0);
        }
    }

    int speed = knight->mob.tickTime & 1;
    if (!mob_move(&knight->mob, knight->xa * speed, knight->ya * speed) || random_next_int(random, 150) == 0) {
        knight->randomWalkTime = 60;
        knight->xa = (random_next_int(random, 3) - 1);
        knight->ya = (random_next_int(random, 3) - 1);
    }
    if (knight->randomWalkTime > 0) --knight->randomWalkTime;
}

void knight_render(Knight* knight, Screen* screen) {
    int xt = 0;
    int yt = 14;
    int flip1 = (knight->mob.walkDist >> 3) & 1;
    int flip2 = (knight->mob.walkDist >> 3) & 1;

    if (knight->mob.dir == 1) xt += 2;
    if (knight->mob.dir > 1) {
        flip1 = 0;
        flip2 = ((knight->mob.walkDist >> 4) & 1);
        if (knight->mob.dir == 2) flip1 = 1;
        xt += 4 + ((knight->mob.walkDist >> 3) & 1) * 2;
    }

    int xo = knight->mob.entity.x - 8;
    int yo = knight->mob.entity.y - 11;
    int col = getColor4(-1, 000, 333, 555); // Heavy iron armor
    if (knight->lvl >= 2) col = getColor4(-1, 110, 440, 553); // Gold knight

    if (knight->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip1, yo + 0, xt + yt * 32, col, flip1);
    render_screen(screen, xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col, flip1);
    render_screen(screen, xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col, flip2);
    render_screen(screen, xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col, flip2);
}

void knight_touchedBy(Knight* knight, Entity* entity) {
    if (entity->type == PLAYER) {
        call_entity_hurt(entity, &knight->mob, knight->lvl * 2 + 1, knight->mob.dir);
    }
}

void knight_die(Knight* knight){
    mob_die(&knight->mob);
    Random* random = &knight->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, (knight->lvl >= 2) ? &goldIngot : &ironIngot);
        itementity_create(item_entity, resource,
            knight->mob.entity.x + random_next_int(random, 11) - 5,
            knight->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(knight->mob.entity.level, &item_entity->entity);
    }

    if (game_player && game_player->mob.entity.level == knight->mob.entity.level) {
        game_player->score += 100 * knight->lvl;
    }
}
