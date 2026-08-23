#ifndef CHICKEN_H
#define CHICKEN_H

#include "mob.h"

typedef struct _Chicken {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
    int eggLayTimer;
    int loveTime;
    int breedCooldown;
    int babyTime;
} Chicken;

void chicken_create(Chicken* chicken);
void chicken_tick(Chicken* chicken);
void chicken_render(Chicken* chicken, Screen* screen);
void chicken_touchedBy(Chicken* chicken, Entity* entity);
void chicken_die(Chicken* chicken);

#endif // CHICKEN_H
