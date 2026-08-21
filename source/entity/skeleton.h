#ifndef SKELETON_H
#define SKELETON_H

#include "mob.h"

typedef struct _Skeleton {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
    int shootDelay;
    int lvl;
} Skeleton;

void skeleton_create(Skeleton* skeleton, int lvl);
void skeleton_tick(Skeleton* skeleton);
void skeleton_render(Skeleton* skeleton, Screen* screen);
void skeleton_touchedBy(Skeleton* skeleton, Entity* entity);
void skeleton_die(Skeleton* skeleton);

#endif // SKELETON_H
