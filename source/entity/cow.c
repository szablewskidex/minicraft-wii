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

    // Peaceful wander
    int speed = cow->mob.tickTime & 1;
    if (!mob_move(&cow->mob, cow->xa * speed, cow->ya * speed) || random_next_int(random, 100) == 0) {
        cow->randomWalkTime = 50;
        cow->xa = (random_next_int(random, 3) - 1);
        cow->ya = (random_next_int(random, 3) - 1);
    }
    if (cow->randomWalkTime > 0) --cow->randomWalkTime;

    animal_tickBreeding((Entity*)cow, &cow->loveTime, &cow->breedCooldown, &cow->xa, &cow->ya);
}

void cow_render(Cow* cow, Screen* screen) {
    int xt = 0;
    int yt = 14;
    int flip1 = (cow->mob.walkDist >> 3) & 1;
    int flip2 = (cow->mob.walkDist >> 3) & 1;

    if (cow->mob.dir == 1) xt += 2;
    if (cow->mob.dir > 1) {
        flip1 = 0;
        flip2 = ((cow->mob.walkDist >> 4) & 1);
        if (cow->mob.dir == 2) flip1 = 1;
        xt += 4 + ((cow->mob.walkDist >> 3) & 1) * 2;
    }

    int xo = cow->mob.entity.x - 8;
    int yo = cow->mob.entity.y - 11;
    int col = getColor4(-1, 211, 432, 555); // Brown / white cow

    if (cow->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip1, yo + 0, xt + yt * 32, col, flip1);
    render_screen(screen, xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col, flip1);
    render_screen(screen, xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col, flip2);
    render_screen(screen, xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col, flip2);
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
