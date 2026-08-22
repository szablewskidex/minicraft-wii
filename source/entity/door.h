#ifndef DOOR_H
#define DOOR_H

#include "furniture.h"

struct _Player;

typedef struct _Door {
    Furniture furniture;
    int isOpen;
} Door;

void door_create(Door* door);
char door_use(Door* door, struct _Player* player, int attackDir);
void door_render(Door* door, Screen* screen);

#endif // DOOR_H
