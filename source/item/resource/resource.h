#ifndef ITEM_RESOURCE_RESOURCE_H_
#define ITEM_RESOURCE_RESOURCE_H_

#include "food_resource.h"
#include "plantable_resource.h"

struct _Player;

typedef enum {
	RES_TYPE_NORMAL = 0,
	RES_TYPE_PLANTABLE = 1,
	RES_TYPE_FOOD = 2
} ResourceType;

typedef struct _Resource{
	char name[16]; // max name len
	int sprite;
	int color;
	ResourceType type;
	res_food food;
	res_plantable plantable;
} Resource;

extern Resource wood, stone, flower, acorn, dirt, sand, cactusFlower, seeds, wheat, bread, apple;
extern Resource coal, ironOre, goldOre, ironIngot, goldIngot;
extern Resource slime, glass, cloth, cloud, gem, leather, boat;
extern Resource leatherArmor, ironArmor, goldArmor, gemArmor;
extern Resource bow, arrow, fishingRod, rawFish, cookedFish, shears, carrot, potato;
extern Resource rawBeef, cookedSteak, rawPork, cookedPork, woodPlank, woodWallItem, egg, torchItem;


void init_resources();
void init_resource(Resource* resource, char* name, int sprite, int color);
char resource_interactOn(Resource* resource, TileID tile, Level* level, int xt, int yt, struct _Player* player, int attackDir);

#endif /* ITEM_RESOURCE_RESOURCE_H_ */
