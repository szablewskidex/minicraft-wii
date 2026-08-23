#ifndef BARREL_H
#define BARREL_H
#include "furniture.h"
#include "inventory.h"

typedef struct _Barrel {
	Furniture furniture;
	Inventory inventory;
} Barrel;

void barrel_create(Barrel* barrel);
char barrel_use(Barrel* barrel, struct _Player* player, int attackDir);
void barrel_free(Barrel* barrel);

#endif // BARREL_H
