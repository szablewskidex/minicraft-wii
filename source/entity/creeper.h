#ifndef CREEPER_H
#define CREEPER_H

#include "mob.h"

typedef struct _Creeper {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
    int fuse;
    int lvl;
} Creeper;

void creeper_create(Creeper* creeper, int lvl);
void creeper_tick(Creeper* creeper);
void creeper_render(Creeper* creeper, Screen* screen);
void creeper_touchedBy(Creeper* creeper, Entity* entity);
void creeper_die(Creeper* creeper);

#endif // CREEPER_H
