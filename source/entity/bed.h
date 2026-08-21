#ifndef BED_H
#define BED_H

#include "furniture.h"

typedef Furniture Bed;

struct _Player;

void bed_create(Bed* bed);
char bed_use(Bed* bed, struct _Player* player, int attackDir);

#endif // BED_H
