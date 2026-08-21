#include "creeper.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "particle/smashparticle.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"
#include "../sound.h"

void creeper_create(Creeper* creeper, int lvl){
    mob_create(&creeper->mob);
    creeper->mob.entity.type = CREEPER;
    creeper->lvl = lvl;
    creeper->mob.entity.x = random_next_int(&creeper->mob.entity.random, 64 * 16);
    creeper->mob.entity.y = random_next_int(&creeper->mob.entity.random, 64 * 16);
    creeper->mob.health = creeper->mob.maxHealth = lvl * 15;
    creeper->randomWalkTime = 0;
    creeper->fuse = 0;
    creeper->xa = creeper->ya = 0;
}

static void creeper_explode(Creeper* creeper) {
    sound_play(SND_EXPLODE);
    int cx = creeper->mob.entity.x;
    int cy = creeper->mob.entity.y;

    // Spawn smash particles
    for (int i = 0; i < 4; ++i) {
        SmashParticle* smash = malloc(sizeof(SmashParticle));
        smashparticle_create(smash, cx + (i%2)*8 - 4, cy + (i/2)*8 - 4);
        level_addEntity(creeper->mob.entity.level, &smash->entity);
    }

    // Hurt nearby entities
    if (game_player && game_player->mob.entity.level == creeper->mob.entity.level) {
        int xd = game_player->mob.entity.x - cx;
        int yd = game_player->mob.entity.y - cy;
        if ((xd * xd + yd * yd) < (32 * 32)) {
            call_entity_hurt((Entity*)game_player, &creeper->mob, 6 * creeper->lvl, creeper->mob.dir);
        }
    }

    // Hurt tiles in explosion radius
    int xt = cx >> 4;
    int yt = cy >> 4;
    for (int y = yt - 1; y <= yt + 1; ++y) {
        for (int x = xt - 1; x <= xt + 1; ++x) {
            if (x >= 0 && y >= 0 && x < creeper->mob.entity.level->w && y < creeper->mob.entity.level->h) {
                TileID t = level_get_tile(creeper->mob.entity.level, x, y);
                tile_hurt(t, creeper->mob.entity.level, x, y, (Mob*)creeper, 50, 0);
            }
        }
    }

    creeper_die(creeper);
}

void creeper_tick(Creeper* creeper) {
    mob_tick(&creeper->mob);
    Random* random = &creeper->mob.entity.random;

    if (game_player && game_player->mob.entity.level == creeper->mob.entity.level) {
        int xd = game_player->mob.entity.x - creeper->mob.entity.x;
        int yd = game_player->mob.entity.y - creeper->mob.entity.y;
        int dist2 = (xd * xd) + (yd * yd);

        if (dist2 < (24 * 24)) {
            // Start hissing / fuse
            if (++creeper->fuse >= 35) {
                creeper_explode(creeper);
                return;
            }
        } else {
            if (creeper->fuse > 0) --creeper->fuse;
        }

        if (dist2 < (60 * 60)) {
            creeper->xa = (xd < 0) ? -1 : ((xd > 0) ? 1 : 0);
            creeper->ya = (yd < 0) ? -1 : ((yd > 0) ? 1 : 0);
        }
    }

    int speed = creeper->mob.tickTime & 1;
    if (!mob_move(&creeper->mob, creeper->xa * speed, creeper->ya * speed) || random_next_int(random, 150) == 0) {
        creeper->randomWalkTime = 60;
        creeper->xa = (random_next_int(random, 3) - 1);
        creeper->ya = (random_next_int(random, 3) - 1);
    }
    if (creeper->randomWalkTime > 0) --creeper->randomWalkTime;
}

void creeper_render(Creeper* creeper, Screen* screen) {
    int xt = 0;
    int yt = 14;
    int flip1 = (creeper->mob.walkDist >> 3) & 1;
    int flip2 = (creeper->mob.walkDist >> 3) & 1;

    if (creeper->mob.dir == 1) xt += 2;
    if (creeper->mob.dir > 1) {
        flip1 = 0;
        flip2 = ((creeper->mob.walkDist >> 4) & 1);
        if (creeper->mob.dir == 2) flip1 = 1;
        xt += 4 + ((creeper->mob.walkDist >> 3) & 1) * 2;
    }

    int xo = creeper->mob.entity.x - 8;
    int yo = creeper->mob.entity.y - 11;
    int col = getColor4(-1, 10, 20, 050); // Creeper green

    if (creeper->fuse > 0 && ((creeper->fuse / 4) % 2 == 1)) {
        col = getColor4(-1, 555, 555, 555); // Flashing fuse
    }
    if (creeper->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip1, yo + 0, xt + yt * 32, col, flip1);
    render_screen(screen, xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col, flip1);
    render_screen(screen, xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col, flip2);
    render_screen(screen, xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col, flip2);
}

void creeper_touchedBy(Creeper* creeper, Entity* entity) {
    if (entity->type == PLAYER) {
        call_entity_hurt(entity, &creeper->mob, creeper->lvl, creeper->mob.dir);
    }
}

void creeper_die(Creeper* creeper){
    mob_die(&creeper->mob);
    Random* random = &creeper->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &coal);
        itementity_create(item_entity, resource,
            creeper->mob.entity.x + random_next_int(random, 11) - 5,
            creeper->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(creeper->mob.entity.level, &item_entity->entity);
    }

    if (game_player && game_player->mob.entity.level == creeper->mob.entity.level) {
        game_player->score += 75 * creeper->lvl;
    }
}
