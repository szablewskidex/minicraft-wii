#include "arrow.h"
#include "_entity_caller.h"
#include "../game.h"
#include "../gfx/color.h"
#include "../sound.h"
#include "particle/smashparticle.h"

void arrow_create(Arrow* arrow, struct _Mob* owner, int x, int y, int dir, int damage) {
    entity_create(&arrow->entity);
    arrow->entity.type = ARROW;
    arrow->entity.x = x;
    arrow->entity.y = y;
    arrow->entity.xr = 3;
    arrow->entity.yr = 3;
    arrow->dir = dir;
    arrow->damage = damage;
    arrow->owner = owner;
    arrow->time = 0;
    arrow->lifeTime = 60; // 1 second range
}

void arrow_tick(Arrow* arrow) {
    ++arrow->time;
    if (arrow->time >= arrow->lifeTime) {
        entity_remove(&arrow->entity);
        return;
    }

    int speed = 5;
    int xa = 0;
    int ya = 0;
    if (arrow->dir == 0) ya = speed;
    if (arrow->dir == 1) ya = -speed;
    if (arrow->dir == 2) xa = -speed;
    if (arrow->dir == 3) xa = speed;

    // Check hit against tiles
    int xt = (arrow->entity.x + xa) >> 4;
    int yt = (arrow->entity.y + ya) >> 4;
    if (xt >= 0 && yt >= 0 && xt < arrow->entity.level->w && yt < arrow->entity.level->h) {
        TileID tile = level_get_tile(arrow->entity.level, xt, yt);
        if (tile == ROCK || tile == HARD_ROCK || tile == TREE) {
            SmashParticle* smash = malloc(sizeof(SmashParticle));
            if (smash) {
                smashparticle_create(smash, arrow->entity.x, arrow->entity.y);
                level_addEntity(arrow->entity.level, &smash->entity);
            }
            entity_remove(&arrow->entity);
            return;
        }
    }

    // Move arrow
    arrow->entity.x += xa;
    arrow->entity.y += ya;

    // Check hit against entities
    ArrayList entities;
    create_arraylist(&entities);
    level_getEntities(arrow->entity.level, &entities, arrow->entity.x - 6, arrow->entity.y - 6, arrow->entity.x + 6, arrow->entity.y + 6);

    for (int i = 0; i < entities.size; ++i) {
        Entity* e = entities.elements[i];
        if (e != &arrow->entity && (Mob*)e != arrow->owner && e->type != ARROW && e->type != EXPORB && e->type != ITEMENTITY) {
            call_entity_hurt(e, arrow->owner, arrow->damage, arrow->dir);
            SmashParticle* smash = malloc(sizeof(SmashParticle));
            if (smash) {
                smashparticle_create(smash, arrow->entity.x, arrow->entity.y);
                level_addEntity(arrow->entity.level, &smash->entity);
            }
            arraylist_remove(&entities);
            entity_remove(&arrow->entity);
            return;
        }
    }

    arraylist_remove(&entities);
}

void arrow_render(Arrow* arrow, Screen* screen) {
    int x = arrow->entity.x;
    int y = arrow->entity.y;
    // Arrow directions: 0=Down, 1=Up, 2=Left, 3=Right
    // Sprites in row 41 at col 16, 17, 18, 19
    int sprite = (16 + (arrow->dir & 3)) + 41 * 32;
    int col = getColor4(-1, 100, 321, 555);

    render_screen(screen, x - 4, y - 4, sprite, col, 0);
}

void arrow_touchedBy(Arrow* arrow, Entity* entity) {
    if (entity != &arrow->entity && (Mob*)entity != arrow->owner && entity->type != ARROW && entity->type != EXPORB && entity->type != ITEMENTITY) {
        call_entity_hurt(entity, arrow->owner, arrow->damage, arrow->dir);
        entity_remove(&arrow->entity);
    }
}
