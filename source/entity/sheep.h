#ifndef SHEEP_H
#define SHEEP_H

#include "mob.h"

typedef struct _Sheep {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
} Sheep;

void sheep_create(Sheep* sheep);
void sheep_tick(Sheep* sheep);
void sheep_render(Sheep* sheep, Screen* screen);
void sheep_touchedBy(Sheep* sheep, Entity* entity);
void sheep_die(Sheep* sheep);

#endif // SHEEP_H
