#ifndef COW_H
#define COW_H

#include "mob.h"

typedef struct _Cow {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
    int loveTime;
    int breedCooldown;
} Cow;

void cow_create(Cow* cow);
void cow_tick(Cow* cow);
void cow_render(Cow* cow, Screen* screen);
void cow_touchedBy(Cow* cow, Entity* entity);
void cow_die(Cow* cow);

#endif // COW_H
