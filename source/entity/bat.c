#include "bat.h"
#include "player.h"
#include "exporb.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../gfx/color.h"
#include "../game.h"
#include "../sound.h"
#include "_entity_caller.h"

void bat_create(Bat* bat) {
    mob_create(&bat->mob);
    bat->mob.entity.type = BAT;
    bat->mob.health = bat->mob.maxHealth = 4;
    bat->mob.entity.xr = 4;
    bat->mob.entity.yr = 4;
    bat->xa = bat->ya = 0;
    bat->randomWalkTime = 0;
}

void bat_tick(Bat* bat) {
    mob_tick(&bat->mob);
    Random* random = &bat->mob.entity.random;

    if (game_player && game_player->mob.entity.level == bat->mob.entity.level) {
        int xd = game_player->mob.entity.x - bat->mob.entity.x;
        int yd = game_player->mob.entity.y - bat->mob.entity.y;
        int d2 = xd * xd + yd * yd;

        if (d2 < 70 * 70) {
            // Aggressive swooping towards player!
            bat->xa = (xd > 0) ? 1 : ((xd < 0) ? -1 : 0);
            bat->ya = (yd > 0) ? 1 : ((yd < 0) ? -1 : 0);
        } else {
            // Erratic flutter
            if (--bat->randomWalkTime <= 0) {
                bat->randomWalkTime = 15 + random_next_int(random, 25);
                bat->xa = random_next_int(random, 3) - 1;
                bat->ya = random_next_int(random, 3) - 1;
            }
        }
    } else {
        if (--bat->randomWalkTime <= 0) {
            bat->randomWalkTime = 15 + random_next_int(random, 25);
            bat->xa = random_next_int(random, 3) - 1;
            bat->ya = random_next_int(random, 3) - 1;
        }
    }

    // Fast flying movement (flapping every tick)
    int speed = 1;
    if (!mob_move(&bat->mob, bat->xa * speed, bat->ya * speed)) {
        bat->randomWalkTime = 0;
    }
}

void bat_render(Bat* bat, Screen* screen) {
    int frame = (bat->mob.tickTime >> 2) & 3; // Fast wing flapping!
    int xt = 8 + frame * 2; // cols 8, 10, 12, 14
    int yt = 36; // Row 36

    int flip = (bat->mob.dir == 2) ? 1 : 0;
    int xo = bat->mob.entity.x - 8;
    int yo = bat->mob.entity.y - 8;
    int col = getColor4(-1, 000, 222, 500); // Slate bat body with glowing red eyes

    if (bat->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void bat_touchedBy(Bat* bat, Entity* entity) {
    if (entity->type == PLAYER) {
        call_entity_hurt(entity, &bat->mob, 1, bat->mob.dir ^ 1);
    }
}

void bat_die(Bat* bat) {
    mob_die(&bat->mob);

    // Drop leather or cloth
    ItemEntity* item_entity = malloc(sizeof(ItemEntity));
    if (item_entity) {
        Item item;
        resourceitem_create(&item, (random_next_int(&bat->mob.entity.random, 2) == 0) ? &leather : &cloth);
        itementity_create(item_entity, item, bat->mob.entity.x, bat->mob.entity.y);
        level_addEntity(bat->mob.entity.level, &item_entity->entity);
    }

    // Exp Orb
    ExpOrb* exp = malloc(sizeof(ExpOrb));
    if (exp) {
        exporb_create(exp, bat->mob.entity.x, bat->mob.entity.y, 4);
        level_addEntity(bat->mob.entity.level, &exp->entity);
    }

    sound_play(SND_MONSTERHURT);
}
