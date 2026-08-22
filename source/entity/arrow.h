#ifndef ARROW_H
#define ARROW_H

#include "entity.h"

struct _Player;
struct _Mob;

typedef struct _Arrow {
    Entity entity;
    int dir;
    int damage;
    int time;
    int lifeTime;
    struct _Mob* owner;
} Arrow;

void arrow_create(Arrow* arrow, struct _Mob* owner, int x, int y, int dir, int damage);
void arrow_tick(Arrow* arrow);
void arrow_render(Arrow* arrow, Screen* screen);
void arrow_touchedBy(Arrow* arrow, Entity* entity);

#endif // ARROW_H
