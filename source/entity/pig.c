#include "pig.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void pig_create(Pig* pig) {
    mob_create(&pig->mob);
    pig->mob.entity.type = PIG;
    pig->mob.entity.x = random_next_int(&pig->mob.entity.random, 64 * 16);
    pig->mob.entity.y = random_next_int(&pig->mob.entity.random, 64 * 16);
    pig->mob.health = pig->mob.maxHealth = 8;
    pig->randomWalkTime = 0;
    pig->xa = pig->ya = 0;
    pig->loveTime = 0;
    pig->breedCooldown = 0;
}

void pig_tick(Pig* pig) {
    mob_tick(&pig->mob);
    Random* random = &pig->mob.entity.random;

    int speed = pig->mob.tickTime & 1;
    if (!mob_move(&pig->mob, pig->xa * speed, pig->ya * speed) || random_next_int(random, 80) == 0) {
        pig->randomWalkTime = 40;
        pig->xa = (random_next_int(random, 3) - 1);
        pig->ya = (random_next_int(random, 3) - 1);
    }
    if (pig->randomWalkTime > 0) --pig->randomWalkTime;

    animal_tickBreeding((Entity*)pig, &pig->loveTime, &pig->breedCooldown, &pig->xa, &pig->ya);
}

void pig_render(Pig* pig, Screen* screen) {
    int frame = (pig->mob.walkDist >> 3) & 3;
    int xt = 8 + frame * 2; // 8, 10, 12, 14
    int yt = 32;

    int flip = (pig->mob.dir == 2) ? 1 : 0;
    int xo = pig->mob.entity.x - 8;
    int yo = pig->mob.entity.y - 11;
    int col = getColor4(-1, 211, 533, 544);

    if (pig->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void pig_touchedBy(Pig* pig, Entity* entity) {
    (void)pig;
    (void)entity;
}

void pig_die(Pig* pig) {
    mob_die(&pig->mob);
    Random* random = &pig->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &rawPork); // Raw pork drop!
        itementity_create(item_entity, resource,
            pig->mob.entity.x + random_next_int(random, 11) - 5,
            pig->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(pig->mob.entity.level, &item_entity->entity);
    }
}
