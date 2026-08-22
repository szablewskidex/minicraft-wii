#ifndef PIG_H
#define PIG_H

#include "mob.h"

typedef struct _Pig {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
} Pig;

void pig_create(Pig* pig);
void pig_tick(Pig* pig);
void pig_render(Pig* pig, Screen* screen);
void pig_touchedBy(Pig* pig, Entity* entity);
void pig_die(Pig* pig);

#endif // PIG_H
