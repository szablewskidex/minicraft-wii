#include "skeleton.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"
#include "spark.h"
#include "airwizard.h"

void skeleton_create(Skeleton* skeleton, int lvl){
    mob_create(&skeleton->mob);
    skeleton->mob.entity.type = SKELETON;
    skeleton->lvl = lvl;
    skeleton->mob.entity.x = random_next_int(&skeleton->mob.entity.random, 64 * 16);
    skeleton->mob.entity.y = random_next_int(&skeleton->mob.entity.random, 64 * 16);
    skeleton->mob.health = skeleton->mob.maxHealth = lvl * 12;
    skeleton->randomWalkTime = 0;
    skeleton->shootDelay = 60;
    skeleton->xa = skeleton->ya = 0;
}

void skeleton_tick(Skeleton* skeleton) {
    mob_tick(&skeleton->mob);
    Random* random = &skeleton->mob.entity.random;

    if (game_player && game_player->mob.entity.level == skeleton->mob.entity.level) {
        int xd = game_player->mob.entity.x - skeleton->mob.entity.x;
        int yd = game_player->mob.entity.y - skeleton->mob.entity.y;
        int dist2 = (xd * xd) + (yd * yd);

        if (dist2 < (70 * 70)) {
            // Keep distance / shoot
            if (dist2 < (40 * 40)) {
                // Back away slightly
                skeleton->xa = (xd > 0) ? -1 : 1;
                skeleton->ya = (yd > 0) ? -1 : 1;
            } else {
                skeleton->xa = skeleton->ya = 0;
            }

            if (--skeleton->shootDelay <= 0) {
                skeleton->shootDelay = 80;
                Spark* spark = malloc(sizeof(Spark));
                spark_create(spark, (AirWizard*)skeleton, (xd > 0 ? 1 : -1) * 2, (yd > 0 ? 1 : -1) * 2);
                spark->entity.x = skeleton->mob.entity.x;
                spark->entity.y = skeleton->mob.entity.y;
                level_addEntity(skeleton->mob.entity.level, &spark->entity);
            }
        }
    }

    int speed = skeleton->mob.tickTime & 1;
    if (!mob_move(&skeleton->mob, skeleton->xa * speed, skeleton->ya * speed) || random_next_int(random, 120) == 0) {
        skeleton->randomWalkTime = 40;
        skeleton->xa = (random_next_int(random, 3) - 1);
        skeleton->ya = (random_next_int(random, 3) - 1);
    }
    if (skeleton->randomWalkTime > 0) --skeleton->randomWalkTime;
}

void skeleton_render(Skeleton* skeleton, Screen* screen) {
    int frame = (skeleton->mob.walkDist >> 3) & 3;
    int xt = 8 + frame * 2; // 8, 10, 12, 14
    int yt = 34;

    int flip = (skeleton->mob.dir == 2) ? 1 : 0;
    int xo = skeleton->mob.entity.x - 8;
    int yo = skeleton->mob.entity.y - 11;
    int col = getColor4(-1, 000, 333, 555); // Bone / archer

    if (skeleton->lvl == 2) col = getColor4(-1, 100, 421, 555);
    if (skeleton->lvl == 3) col = getColor4(-1, 110, 541, 555);

    if (skeleton->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void skeleton_touchedBy(Skeleton* skeleton, Entity* entity) {
    if (entity->type == PLAYER) {
        call_entity_hurt(entity, &skeleton->mob, skeleton->lvl + 1, skeleton->mob.dir);
    }
}

void skeleton_die(Skeleton* skeleton){
    mob_die(&skeleton->mob);
    Random* random = &skeleton->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &cloth);
        itementity_create(item_entity, resource,
            skeleton->mob.entity.x + random_next_int(random, 11) - 5,
            skeleton->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(skeleton->mob.entity.level, &item_entity->entity);
    }

    if (game_player && game_player->mob.entity.level == skeleton->mob.entity.level) {
        int xd = game_player->mob.entity.x - skeleton->mob.entity.x;
        int yd = game_player->mob.entity.y - skeleton->mob.entity.y;
        if (xd * xd + yd * yd < 120 * 120) {
            game_player->score += 60 * skeleton->lvl;
            player_addExp(game_player, 30 * skeleton->lvl);
        }
    }
}
