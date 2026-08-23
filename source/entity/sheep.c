#include "sheep.h"
#include "../gfx/screen.h"
#include "_entity_caller.h"
#include "itementity.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../game.h"
#include "../gfx/color.h"

void sheep_create(Sheep* sheep) {
    mob_create(&sheep->mob);
    sheep->mob.entity.type = SHEEP;
    sheep->mob.entity.x = random_next_int(&sheep->mob.entity.random, 64 * 16);
    sheep->mob.entity.y = random_next_int(&sheep->mob.entity.random, 64 * 16);
    sheep->mob.health = sheep->mob.maxHealth = 8;
    sheep->randomWalkTime = 0;
    sheep->xa = sheep->ya = 0;
    sheep->hasWool = 1;
    sheep->regrowTimer = 0;
    sheep->loveTime = 0;
    sheep->breedCooldown = 0;
}

void sheep_tick(Sheep* sheep) {
    mob_tick(&sheep->mob);
    Random* random = &sheep->mob.entity.random;

    int speed = (sheep->breedCooldown > 0) ? (sheep->mob.tickTime % 4 == 0) : (sheep->mob.tickTime & 1);
    if (!mob_move(&sheep->mob, sheep->xa * speed, sheep->ya * speed) || random_next_int(random, 90) == 0) {
        sheep->randomWalkTime = 45;
        sheep->xa = (random_next_int(random, 3) - 1);
        sheep->ya = (random_next_int(random, 3) - 1);
    }
    if (sheep->randomWalkTime > 0) --sheep->randomWalkTime;

    if (!sheep->hasWool) {
        if (sheep->regrowTimer > 0) {
            --sheep->regrowTimer;
        } else {
            sheep->hasWool = 1;
        }
    }

    animal_tickBreeding((Entity*)sheep, &sheep->loveTime, &sheep->breedCooldown, &sheep->xa, &sheep->ya);
}

void sheep_render(Sheep* sheep, Screen* screen) {
    int frame = (sheep->mob.walkDist >> 3) & 3;
    int xt = 16 + frame * 2; // 16, 18, 20, 22
    int yt = 32;

    int flip = (sheep->mob.dir == 2) ? 1 : 0;
    int xo = sheep->mob.entity.x - 8;
    int yo = sheep->mob.entity.y - 11;
    int col = getColor4(-1, 111, 333, 555);

    if (sheep->mob.hurtTime > 0) col = getColor4(-1, 555, 555, 555);

    render_screen(screen, xo + 8 * flip, yo + 0, xt + yt * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 0, xt + 1 + yt * 32, col, flip);
    render_screen(screen, xo + 8 * flip, yo + 8, xt + (yt + 1) * 32, col, flip);
    render_screen(screen, xo + 8 - 8 * flip, yo + 8, xt + 1 + (yt + 1) * 32, col, flip);
}

void sheep_touchedBy(Sheep* sheep, Entity* entity) {
    (void)sheep;
    (void)entity;
}

void sheep_die(Sheep* sheep) {
    mob_die(&sheep->mob);
    Random* random = &sheep->mob.entity.random;
    int count = random_next_int(random, 2) + 1;

    for (int i = 0; i < count; ++i) {
        ItemEntity* item_entity = malloc(sizeof(ItemEntity));
        if (!item_entity) continue;
        Item resource;
        resourceitem_create(&resource, &cloth); // Wool / cloth drop!
        itementity_create(item_entity, resource,
            sheep->mob.entity.x + random_next_int(random, 11) - 5,
            sheep->mob.entity.y + random_next_int(random, 11) - 5);
        level_addEntity(sheep->mob.entity.level, &item_entity->entity);
    }
}
