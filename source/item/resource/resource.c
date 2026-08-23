#include <stdio.h>
#include <string.h>
#include "resource.h"
#include "plantable_resource.h"
#include "food_resource.h"
#include "../resourceitem.h"
#include "../../gfx/color.h"
#include "../../entity/player.h"
#include "../../entity/arrow.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/textparticle.h"
#include "../../sound.h"
#include "../../inputhandler.h"
#include "../../lang.h"

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
Resource ironArmor;
Resource goldArmor;
Resource gemArmor;

Resource bow;
Resource arrow;
Resource fishingRod;
Resource rawFish;
Resource cookedFish;
Resource shears;
Resource carrot;
Resource potato;
Resource rawBeef;
Resource cookedSteak;
Resource rawPork;
Resource cookedPork;
Resource woodPlank;
Resource woodWallItem;
Resource egg;


void init_resource(Resource* resource, char* name, int sprite, int color) {
	memset(resource->name, 0, sizeof(resource->name));
	strncpy(resource->name, name, 15);
	resource->name[15] = '\0';
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
TileID crop_sources[] = {FARMLAND};


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
	init_resource(&boat, "Boat", 5 + 41 * 32, getColor4(-1, 100, 321, 431));

	// All 4 Armors (Row 41)
	init_resource(&leatherArmor, "L.Armor", 0 + 41 * 32, getColor4(-1, 100, 321, 431));
	init_resource(&ironArmor, "I.Armor", 1 + 41 * 32, getColor4(-1, 100, 322, 555));
	init_resource(&goldArmor, "G.Armor", 2 + 41 * 32, getColor4(-1, 110, 440, 553));
	init_resource(&gemArmor, "Gem Armor", 3 + 41 * 32, getColor4(-1, 101, 244, 455));

	// New Tools & Items (Row 41)
	init_resource(&bow, "Bow", 6 + 41 * 32, getColor4(-1, 100, 321, 431));
	init_resource(&arrow, "Arrow", 7 + 41 * 32, getColor4(-1, 100, 321, 555));
	init_resource(&fishingRod, "Rod", 8 + 41 * 32, getColor4(-1, 100, 321, 444));
	init_food_resource(&rawFish, "Raw Fish", 9 + 41 * 32, getColor4(-1, 111, 233, 445), 1, 3);
	init_food_resource(&cookedFish, "C.Fish", 10 + 41 * 32, getColor4(-1, 100, 321, 432), 4, 10);
	init_resource(&shears, "Shears", 11 + 41 * 32, getColor4(-1, 100, 333, 555));
	init_food_resource(&carrot, "Carrot", 12 + 41 * 32, getColor4(-1, 10, 530, 50), 2, 4);
	init_food_resource(&potato, "Potato", 13 + 41 * 32, getColor4(-1, 100, 431, 542), 2, 4);
	init_food_resource(&rawBeef, "Raw Beef", 14 + 41 * 32, getColor4(-1, 100, 400, 533), 2, 4);
	init_food_resource(&cookedSteak, "Steak", 15 + 41 * 32, getColor4(-1, 100, 310, 421), 6, 12);
	init_food_resource(&rawPork, "Raw Pork", 20 + 41 * 32, getColor4(-1, 211, 422, 544), 2, 4);
	init_food_resource(&cookedPork, "C.Pork", 21 + 41 * 32, getColor4(-1, 100, 320, 542), 5, 10);
	init_resource(&woodPlank, "Plank", 22 + 41 * 32, getColor4(-1, 100, 321, 431));
	init_resource(&woodWallItem, "Wood Wall", 23 + 41 * 32, getColor4(-1, 100, 321, 431));

	// Egg in row 4 col 14 (Exact user selection!)
	init_food_resource(&egg, "Egg", 14 + 4 * 32, getColor4(-1, 310, 542, 555), 1, 2);

	init_plantable_resource(&cloud, "cloud", 2 + 4 * 32, getColor4(-1, 222, 555, 444), CLOUD, cloud_sources, sizeof(cloud_sources)/sizeof(TileID));
	init_resource(&gem, "gem", 13 + 4 * 32, getColor4(-1, 101, 404, 545));
}


char resource_interactOn(Resource* resource, TileID tile, Level* level, int xt, int yt, Player* player, int attackDir) {
	if (resource->add.plantable.sourceTiles) {
		for (int i = 0; i < resource->add.plantable.sourceTilesSize; ++i) {
			if (tile == resource->add.plantable.sourceTiles[i]) {
				level_set_tile(level, xt, yt, resource->add.plantable.targetTile, 0);
				sound_play(SND_CONFIRM);
				return 1;
			}
		}
		return 0;
	} else if (resource == &bread || resource == &apple || resource == &rawFish || resource == &cookedFish || resource == &carrot || resource == &potato || resource == &rawBeef || resource == &cookedSteak || resource == &rawPork || resource == &cookedPork || resource == &egg) {
		if (player->mob.health < player->mob.maxHealth && player_payStamina(player, resource->add.food.staminaCost)) {
			mob_heal(&player->mob, resource->add.food.heal);
			sound_play(SND_CONFIRM);
			return 1;
		}
		return 0;
	} else if (resource == &woodPlank) {
		if (tile == DIRT || tile == HOLE || tile == GRASS || tile == SAND) {
			level_set_tile(level, xt, yt, WOOD_FLOOR, 0);
			sound_play(SND_CONFIRM);
			input_rumble(4);
			return 1;
		}
		return 0;
	} else if (resource == &woodWallItem) {
		if (tile == DIRT || tile == GRASS || tile == SAND || tile == WOOD_FLOOR || tile == HOLE) {
			level_set_tile(level, xt, yt, WOOD_WALL, 0);
			sound_play(SND_CONFIRM);
			input_rumble(4);
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
	} else if (resource == &bow) {
		// Find arrow in inventory
		Item* arrowItem = inventory_findResource(&player->inventory, &arrow);
		if (arrowItem && player_payStamina(player, 2)) {
			// Consume 1 arrow
			if (arrowItem->id == RESOURCE) {
				resourceitem_interactOn(arrowItem, tile, level, xt, yt, player, attackDir);
			}
			Arrow* arrow_entity = malloc(sizeof(Arrow));
			if (arrow_entity) {
				arrow_create(arrow_entity, (Mob*)player, player->mob.entity.x, player->mob.entity.y, player->mob.dir, 12);
				level_addEntity(level, &arrow_entity->entity);
			}
			sound_play(SND_MONSTERHURT);
			input_rumble(3);
			return 1;
		}
		return 0;
	} else if (resource == &fishingRod) {
		if (tile == WATER && player_payStamina(player, 2)) {
			int r = random_next_int(&player->mob.entity.random, 100);
			Resource* catchRes = &rawFish;
			if (r < 65) catchRes = &rawFish;
			else if (r < 80) catchRes = &ironIngot;
			else if (r < 90) catchRes = &leather;
			else catchRes = &gem;

			Item catchItem;
			resourceitem_create(&catchItem, catchRes);
			inventory_addItem(&player->inventory, &catchItem);

			sound_play(SND_PICKUP);
			input_rumble(6);

			TextParticle* text_p = malloc(sizeof(TextParticle));
			if (text_p) {
				char msg[32];
				sprintf(msg, "+%s", lang_translate_item(catchRes->name));
				textparticle_create(text_p, strdup(msg), player->mob.entity.x, player->mob.entity.y - 8, getColor4(-1, 550, 550, 550));
				level_addEntity(level, &text_p->entity);
			}
			return 1;
		}
		return 0;
	} else if (resource == &shears) {
		// Check nearby sheep to shear
		ArrayList entities;
		create_arraylist(&entities);
		level_getEntities(level, &entities, player->mob.entity.x - 16, player->mob.entity.y - 16, player->mob.entity.x + 16, player->mob.entity.y + 16);
		for (int i = 0; i < entities.size; ++i) {
			Entity* e = entities.elements[i];
			if (e->type == SHEEP) {
				// Drop wool / cloth!
				Item woolItem;
				resourceitem_create(&woolItem, &cloth);
				inventory_addItem(&player->inventory, &woolItem);
				inventory_addItem(&player->inventory, &woolItem);

				sound_play(SND_CONFIRM);
				input_rumble(4);

				TextParticle* text_p = malloc(sizeof(TextParticle));
				if (text_p) {
					textparticle_create(text_p, strdup("+2 Wool"), e->x, e->y - 8, getColor4(-1, 555, 555, 555));
					level_addEntity(level, &text_p->entity);
				}
				arraylist_remove(&entities);
				return 1;
			}
		}
		arraylist_remove(&entities);
		return 0;
	} else if (resource == &wheat || resource == &seeds || resource == &carrot) {
		// Feed animals to breed
		ArrayList entities;
		create_arraylist(&entities);
		level_getEntities(level, &entities, player->mob.entity.x - 16, player->mob.entity.y - 16, player->mob.entity.x + 16, player->mob.entity.y + 16);
		for (int i = 0; i < entities.size; ++i) {
			Entity* e = entities.elements[i];
			if (e->type == COW || e->type == SHEEP || e->type == CHICKEN || e->type == PIG) {
				sound_play(SND_CONFIRM);
				input_rumble(3);

				TextParticle* text_p = malloc(sizeof(TextParticle));
				if (text_p) {
					textparticle_create(text_p, strdup("<3"), e->x, e->y - 10, getColor4(-1, 500, 500, 555));
					level_addEntity(level, &text_p->entity);
				}
				player_addExp(player, 10);
				arraylist_remove(&entities);
				return 1;
			}
		}
		arraylist_remove(&entities);
		return 0;
	} else if (resource == &leatherArmor || resource == &ironArmor || resource == &goldArmor || resource == &gemArmor) {
		player->armor = resource;
		if (resource == &leatherArmor) player->armorDefense = 1;
		else if (resource == &ironArmor) player->armorDefense = 2;
		else if (resource == &goldArmor) player->armorDefense = 3;
		else if (resource == &gemArmor) player->armorDefense = 4;

		sound_play(SND_CONFIRM);
		return 1;
	}
	return 0;
}
