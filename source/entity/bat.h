#ifndef BAT_H
#define BAT_H

#include "mob.h"

typedef struct _Bat {
    Mob mob;
    int xa, ya;
    int randomWalkTime;
} Bat;

void bat_create(Bat* bat);
void bat_tick(Bat* bat);
void bat_render(Bat* bat, Screen* screen);
void bat_touchedBy(Bat* bat, Entity* entity);
void bat_die(Bat* bat);

#endif // BAT_H
