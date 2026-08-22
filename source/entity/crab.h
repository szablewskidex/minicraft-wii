#ifndef CRAB_H
#define CRAB_H

#include "mob.h"

typedef struct _Crab {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
} Crab;

void crab_create(Crab* crab);
void crab_tick(Crab* crab);
void crab_render(Crab* crab, Screen* screen);
void crab_touchedBy(Crab* crab, Entity* entity);
void crab_die(Crab* crab);

#endif // CRAB_H
