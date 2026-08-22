#ifndef FROG_H
#define FROG_H

#include "mob.h"

typedef struct _Frog {
    Mob mob;
    int xa, ya;
    int jumpTime;
} Frog;

void frog_create(Frog* frog);
void frog_tick(Frog* frog);
void frog_render(Frog* frog, Screen* screen);
void frog_touchedBy(Frog* frog, Entity* entity);
void frog_die(Frog* frog);

#endif // FROG_H
