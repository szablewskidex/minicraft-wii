#include "crab.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void crab_create(Crab* crab) {
    mob_create(&crab->mob);
    crab->mob.entity.type = CRAB;
    crab->mob.entity.x = random_next_int(&crab->mob.entity.random, 64 * 16);
    crab->mob.entity.y = random_next_int(&crab->mob.entity.random, 64 * 16);
    crab->mob.health = crab->mob.maxHealth = 6;
    crab->randomWalkTime = 0;
    crab->xa = crab->ya = 0;
}

void crab_tick(Crab* crab) {
    mob_tick(&crab->mob);
    Random* random = &crab->mob.entity.random;

    // Crab walks sideways
    int speed = crab->mob.tickTime & 1;
    if (!mob_move(&crab->mob, crab->xa * speed, crab->ya * speed) || random_next_int(random, 60) == 0) {
        crab->randomWalkTime = 30;
        crab->xa = (random_next_int(random, 3) - 1);
        crab->ya = (random_next_int(random, 3) - 1);
    }
    if (crab->randomWalkTime > 0) --crab->randomWalkTime;
}

void crab_render(Crab* crab, Screen* screen) {
    int frame = (crab->mob.walkDist >> 3) & 3;
    int xt = 24 + frame * 2; // 24, 26, 28, 30
    int yt = 32;

    int flip = (crab->mob.dir == 2) ? 1 : 0;
    int xo = crab->mob.entity.x - 8;
    int yo = crab->mob.entity.y - 11;
    int col = getColor4(-1, 100, 500, 520);

    if (crab->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void crab_touchedBy(Crab* crab, Entity* entity) {
    (void)crab;
    (void)entity;
}

void crab_die(Crab* crab) {
    mob_die(&crab->mob);
    Random* random = &crab->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &slime);
        itementity_create(item_entity, resource,
            crab->mob.entity.x + random_next_int(random, 11) - 5,
            crab->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(crab->mob.entity.level, &item_entity->entity);
    }
}
