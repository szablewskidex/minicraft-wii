#include "cow.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void cow_create(Cow* cow){
    mob_create(&cow->mob);
    cow->mob.entity.type = COW;
    cow->mob.entity.x = random_next_int(&cow->mob.entity.random, 64 * 16);
    cow->mob.entity.y = random_next_int(&cow->mob.entity.random, 64 * 16);
    cow->mob.health = cow->mob.maxHealth = 8;
    cow->randomWalkTime = 0;
    cow->xa = cow->ya = 0;
    cow->loveTime = 0;
    cow->breedCooldown = 0;
}

void cow_tick(Cow* cow) {
    mob_tick(&cow->mob);
    Random* random = &cow->mob.entity.random;

    if (cow->breedCooldown > 0) {
        // Satiated / grazing mode
        if (cow->randomWalkTime > 0) {
            --cow->randomWalkTime;
            int speed = (cow->mob.tickTime % 4 == 0);
            mob_move(&cow->mob, cow->xa * speed, cow->ya * speed);
        } else {
            cow->xa = cow->ya = 0;
            if (random_next_int(random, 120) == 0) {
                cow->randomWalkTime = 15;
                cow->xa = (random_next_int(random, 3) - 1);
                cow->ya = (random_next_int(random, 3) - 1);
            }
        }
    } else if (cow->loveTime <= 0) {
        // Normal wandering
        if (cow->randomWalkTime > 0) {
            --cow->randomWalkTime;
            int speed = cow->mob.tickTime & 1;
            if (!mob_move(&cow->mob, cow->xa * speed, cow->ya * speed)) {
                cow->randomWalkTime = 0;
            }
        } else {
            cow->xa = cow->ya = 0;
            if (random_next_int(random, 50) == 0) {
                cow->randomWalkTime = 35;
                cow->xa = (random_next_int(random, 3) - 1);
                cow->ya = (random_next_int(random, 3) - 1);
            }
        }
    }

    animal_tickBreeding((Entity*)cow, &cow->loveTime, &cow->breedCooldown, &cow->xa, &cow->ya);
}

void cow_render(Cow* cow, Screen* screen) {
    int frame = (cow->mob.walkDist >> 3) & 3;
    int xt = 8 + frame * 2; // cols 8, 10, 12, 14
    int yt = 50; // Madboar row 50 in spritesheet

    int flip = (cow->mob.dir == 2) ? 1 : 0;
    int xo = cow->mob.entity.x - 8;
    int yo = cow->mob.entity.y - 11;
    int col = getColor4(-1, 000, 321, 555); // Rich boar colors with red eyes

    if (cow->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void cow_touchedBy(Cow* cow, Entity* entity) {
    (void)cow;
    (void)entity;
}

void cow_die(Cow* cow){
    mob_die(&cow->mob);
    Random* random = &cow->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &leather); // Drops leather!
        itementity_create(item_entity, resource,
            cow->mob.entity.x + random_next_int(random, 11) - 5,
            cow->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(cow->mob.entity.level, &item_entity->entity);
    }

    ItemEntity* beef_entity = malloc(sizeof(ItemEntity));
    if (beef_entity) {
        Item beef;
        resourceitem_create(&beef, &rawBeef); // Raw beef drop!
        itementity_create(beef_entity, beef,
            cow->mob.entity.x, cow->mob.entity.y);
        level_addEntity(cow->mob.entity.level, &beef_entity->entity);
    }
}
