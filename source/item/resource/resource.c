#include <stdio.h>
#include <string.h>
#include "resource.h"
#include "plantable_resource.h"
#include "food_resource.h"
#include "../../gfx/color.h"
#include "../../entity/player.h"

Resource wood;
Resource stone;
Resource flower;
Resource acorn;
Resource dirt;
Resource sand;
Resource cactusFlower;
Resource seeds;
Resource wheat;
Resource bread;
Resource apple;

Resource coal;
Resource ironOre;
Resource goldOre;
Resource ironIngot;
Resource goldIngot;

Resource slime;
Resource glass;
Resource cloth;
Resource cloud;
Resource gem;
Resource leather;
Resource boat;
Resource leatherArmor;


void init_resource(Resource* resource, char* name, int sprite, int color) {
	memset(resource->name, 0, sizeof(resource->name));
	strncpy(resource->name, name, 6);
	resource->name[6] = '\0';
	resource->sprite = sprite;
	resource->color = color;
}


TileID flower_sources[] = {GRASS};
TileID acorn_sources[] = {GRASS};
TileID dirt_sources[] = {HOLE, WATER, LAVA};
TileID sand_sources[] = {GRASS, DIRT};
TileID cactus_sources[] = {SAND};
TileID seeds_sources[] = {FARMLAND};
TileID cloud_sources[] = {INFINITE_FALL};
TileID stone_sources[] = {GRASS, DIRT, SAND, FARMLAND, HOLE, WATER, LAVA};


void init_resources() {
	init_resource(&wood, "Wood", 1 + 4 * 32, getColor4(-1, 200, 531, 430));
	init_plantable_resource(&stone, "Stone", 2 + 4 * 32, getColor4(-1, 111, 333, 555), ROCK, stone_sources, sizeof(stone_sources)/sizeof(TileID));

	init_plantable_resource(&flower, "Flower", 0 + 4 * 32, getColor4(-1, 10, 444, 330), FLOWER, flower_sources, sizeof(flower_sources)/sizeof(TileID));
	init_plantable_resource(&acorn, "Acorn", 3 + 4 * 32, getColor4(-1, 100, 531, 320), TREE_SAPLING, acorn_sources, sizeof(acorn_sources)/sizeof(TileID));
	init_plantable_resource(&dirt, "Dirt", 2 + 4 * 32, getColor4(-1, 100, 322, 432), DIRT, dirt_sources, sizeof(dirt_sources)/sizeof(TileID));
	init_plantable_resource(&sand, "Sand", 2 + 4 * 32, getColor4(-1, 110, 440, 550), SAND, sand_sources, sizeof(sand_sources)/sizeof(TileID));
	init_plantable_resource(&cactusFlower, "Cactus", 4 + 4 * 32, getColor4(-1, 10, 40, 50), CACTUS_SAPLING, cactus_sources, sizeof(cactus_sources)/sizeof(TileID));
	init_plantable_resource(&seeds, "Seeds", 5 + 4 * 32, getColor4(-1, 10, 40, 50), WHEAT, seeds_sources, sizeof(seeds_sources)/sizeof(TileID));

	init_resource(&wheat, "Wheat", 6 + 4 * 32, getColor4(-1, 110, 330, 550));

	init_food_resource(&bread, "Bread", 8 + 4 * 32, getColor4(-1, 110, 330, 550), 2, 5);
	init_food_resource(&apple, "Apple", 9 + 4 * 32, getColor4(-1, 100, 300, 500), 1, 5);

	init_resource(&coal, "COAL", 10 + 4 * 32, getColor4(-1, 000, 111, 111));
	init_resource(&ironOre, "I.ORE", 10 + 4 * 32, getColor4(-1, 100, 322, 544));
	init_resource(&goldOre, "G.ORE", 10 + 4 * 32, getColor4(-1, 110, 440, 553));
	init_resource(&ironIngot, "IRON", 11 + 4 * 32, getColor4(-1, 100, 322, 544));
	init_resource(&goldIngot, "GOLD", 11 + 4 * 32, getColor4(-1, 110, 330, 553));

	init_resource(&slime, "SLIME", 10 + 4 * 32, getColor4(-1, 10, 30, 50));
	init_resource(&glass, "glass", 12 + 4 * 32, getColor4(-1, 555, 555, 555));
	init_resource(&cloth, "cloth", 1 + 4 * 32, getColor4(-1, 25, 252, 141));
	init_resource(&leather, "Hide", 6 + 4 * 32, getColor4(-1, 100, 310, 420));
	init_resource(&boat, "Boat", 8 + 4 * 32, getColor4(-1, 100, 321, 431));
	init_resource(&leatherArmor, "Armor", 7 + 4 * 32, getColor4(-1, 100, 310, 420));

	init_plantable_resource(&cloud, "cloud", 2 + 4 * 32, getColor4(-1, 222, 555, 444), CLOUD, cloud_sources, sizeof(cloud_sources)/sizeof(TileID));

	init_resource(&gem, "gem", 13 + 4 * 32, getColor4(-1, 101, 404, 545));
}

#include "../../sound.h"

char resource_interactOn(Resource* resource, TileID tile, Level* level, int xt, int yt, Player* player, int attackDir) {
	if (resource == &stone || resource == &cloud || resource == &flower || resource == &acorn || resource == &dirt || resource == &sand || resource == &cactusFlower || resource == &seeds) {
		for (int i = 0; i < resource->add.plantable.sourceTilesSize; ++i) {
			if (resource->add.plantable.sourceTiles[i] == tile) {
				if (resource->add.plantable.targetTile == ROCK) {
					int px0 = player->mob.entity.x - player->mob.entity.xr;
					int px1 = player->mob.entity.x + player->mob.entity.xr;
					int py0 = player->mob.entity.y - player->mob.entity.yr;
					int py1 = player->mob.entity.y + player->mob.entity.yr;
					int tx0 = xt * 16;
					int tx1 = xt * 16 + 15;
					int ty0 = yt * 16;
					int ty1 = yt * 16 + 15;
					if (px1 >= tx0 && px0 <= tx1 && py1 >= ty0 && py0 <= ty1) {
						return 0;
					}
				}
				level_set_tile(level, xt, yt, resource->add.plantable.targetTile, 0);
				sound_play(SND_CONFIRM);
				return 1;
			}
		}
		return 0;

	} else if (resource == &bread || resource == &apple) {
		if (player->mob.health < player->mob.maxHealth && player_payStamina(player, resource->add.food.staminaCost)) {
			mob_heal(&player->mob, resource->add.food.heal);
			sound_play(SND_CONFIRM);
			return 1;
		}
		return 0;
	} else if (resource == &boat) {
		if (tile == WATER) {
			player->mob.entity.x = xt * 16 + 8;
			player->mob.entity.y = yt * 16 + 8;
			sound_play(SND_CONFIRM);
			return 1;
		}
		return 0;
	}
	return 0;
}
