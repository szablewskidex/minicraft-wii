#include "chicken.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"
#include "../sound.h"

void chicken_create(Chicken* chicken) {
    mob_create(&chicken->mob);
    chicken->mob.entity.type = CHICKEN;
    chicken->mob.entity.x = random_next_int(&chicken->mob.entity.random, 64 * 16);
    chicken->mob.entity.y = random_next_int(&chicken->mob.entity.random, 64 * 16);
    chicken->mob.health = chicken->mob.maxHealth = 4;
    chicken->randomWalkTime = 0;
    chicken->xa = chicken->ya = 0;
    chicken->eggLayTimer = random_next_int(&chicken->mob.entity.random, 3600) + 3600; // 1-2 minutes
    chicken->loveTime = 0;
    chicken->breedCooldown = 0;
    chicken->babyTime = 0;
}

void chicken_tick(Chicken* chicken) {
    mob_tick(&chicken->mob);
    Random* random = &chicken->mob.entity.random;

    if (chicken->breedCooldown > 0) {
        // Satiated / pecking grass mode - mostly stationary, very slow small steps
        if (chicken->randomWalkTime > 0) {
            --chicken->randomWalkTime;
            int speed = (chicken->mob.tickTime % 4 == 0);
            mob_move(&chicken->mob, chicken->xa * speed, chicken->ya * speed);
        } else {
            chicken->xa = chicken->ya = 0;
            if (random_next_int(random, 120) == 0) {
                chicken->randomWalkTime = 12;
                chicken->xa = (random_next_int(random, 3) - 1);
                chicken->ya = (random_next_int(random, 3) - 1);
            }
        }
    } else if (chicken->loveTime <= 0) {
        // Normal wandering
        if (chicken->randomWalkTime > 0) {
            --chicken->randomWalkTime;
            int speed = chicken->mob.tickTime & 1;
            if (!mob_move(&chicken->mob, chicken->xa * speed, chicken->ya * speed)) {
                chicken->randomWalkTime = 0;
            }
        } else {
            chicken->xa = chicken->ya = 0;
            if (random_next_int(random, 40) == 0) {
                chicken->randomWalkTime = 25;
                chicken->xa = (random_next_int(random, 3) - 1);
                chicken->ya = (random_next_int(random, 3) - 1);
            }
        }
    }

    // Lay an egg periodically!
    if (--chicken->eggLayTimer <= 0) {
        chicken->eggLayTimer = random_next_int(random, 3600) + 3600;
        if (chicken->mob.entity.level) {
            ItemEntity* item_entity = malloc(sizeof(ItemEntity));
            if (item_entity) {
                Item item;
                resourceitem_create(&item, &egg);
                itementity_create(item_entity, item, chicken->mob.entity.x, chicken->mob.entity.y);
                level_addEntity(chicken->mob.entity.level, &item_entity->entity);
                sound_play(SND_CONFIRM);
            }
        }
    }

    if (chicken->babyTime > 0) --chicken->babyTime;

    animal_tickBreeding((Entity*)chicken, &chicken->loveTime, &chicken->breedCooldown, &chicken->xa, &chicken->ya);
}

void chicken_render(Chicken* chicken, Screen* screen) {
    int frame = (chicken->mob.walkDist >> 3) & 3;
    int xt = frame * 2; // 0, 2, 4, 6
    int yt = (chicken->babyTime > 0) ? 50 : 32; // Baby chick row 50, adult chicken row 32

    int flip = (chicken->mob.dir == 2) ? 1 : 0;
    int xo = chicken->mob.entity.x - 8;
    int yo = chicken->mob.entity.y - 11;
    int col = (chicken->babyTime > 0) ? getColor4(-1, 000, 540, 550) : getColor4(-1, 000, 510, 555);

    if (chicken->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void chicken_touchedBy(Chicken* chicken, Entity* entity) {
    (void)chicken;
    (void)entity;
}

void chicken_die(Chicken* chicken) {
    mob_die(&chicken->mob);
    Random* random = &chicken->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &seeds);
        itementity_create(item_entity, resource,
            chicken->mob.entity.x + random_next_int(random, 11) - 5,
            chicken->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(chicken->mob.entity.level, &item_entity->entity);
    }

    ItemEntity* egg_entity = malloc(sizeof(ItemEntity));
    if (egg_entity) {
        Item egg_item;
        resourceitem_create(&egg_item, &egg);
        itementity_create(egg_entity, egg_item, chicken->mob.entity.x, chicken->mob.entity.y);
        level_addEntity(chicken->mob.entity.level, &egg_entity->entity);
    }
}
