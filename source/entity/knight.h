#ifndef KNIGHT_H
#define KNIGHT_H

#include "mob.h"

typedef struct _Knight {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
    int lvl;
} Knight;

void knight_create(Knight* knight, int lvl);
void knight_tick(Knight* knight);
void knight_render(Knight* knight, Screen* screen);
void knight_touchedBy(Knight* knight, Entity* entity);
void knight_die(Knight* knight);

#endif // KNIGHT_H
