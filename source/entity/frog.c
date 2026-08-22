#include "frog.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void frog_create(Frog* frog) {
    mob_create(&frog->mob);
    frog->mob.entity.type = FROG;
    frog->mob.entity.x = random_next_int(&frog->mob.entity.random, 64 * 16);
    frog->mob.entity.y = random_next_int(&frog->mob.entity.random, 64 * 16);
    frog->mob.health = frog->mob.maxHealth = 4;
    frog->jumpTime = 0;
    frog->xa = frog->ya = 0;
}

void frog_tick(Frog* frog) {
    mob_tick(&frog->mob);
    Random* random = &frog->mob.entity.random;

    // Frog leaps periodically
    if (frog->jumpTime <= 0) {
        if (random_next_int(random, 40) == 0) {
            frog->jumpTime = 20;
            frog->xa = (random_next_int(random, 3) - 1);
            frog->ya = (random_next_int(random, 3) - 1);
        }
    } else {
        --frog->jumpTime;
        mob_move(&frog->mob, frog->xa * 2, frog->ya * 2);
    }
}

void frog_render(Frog* frog, Screen* screen) {
    int frame = (frog->jumpTime > 0) ? (((20 - frog->jumpTime) >> 2) & 3) : 0;
    int xt = frame * 2; // 0, 2, 4, 6
    int yt = 34;

    int flip = (frog->mob.dir == 2) ? 1 : 0;
    int xo = frog->mob.entity.x - 8;
    int yo = frog->mob.entity.y - 11;
    int col = getColor4(-1, 020, 141, 453);

    if (frog->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void frog_touchedBy(Frog* frog, Entity* entity) {
    (void)frog;
    (void)entity;
}

void frog_die(Frog* frog) {
    mob_die(&frog->mob);
    Random* random = &frog->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &slime);
        itementity_create(item_entity, resource,
            frog->mob.entity.x + random_next_int(random, 11) - 5,
            frog->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(frog->mob.entity.level, &item_entity->entity);
    }
}
