#ifndef EXPORB_H
#define EXPORB_H

#include "entity.h"

struct _Player;

typedef struct _ExpOrb {
    Entity entity;
    int expValue;
    int time;
    int lifeTime;
    float xx, yy, zz;
    float xa, ya, za;
} ExpOrb;

void exporb_create(ExpOrb* orb, int x, int y, int expValue);
void exporb_tick(ExpOrb* orb);
void exporb_render(ExpOrb* orb, Screen* screen);
void exporb_touchedBy(ExpOrb* orb, Entity* entity);

#endif // EXPORB_H
