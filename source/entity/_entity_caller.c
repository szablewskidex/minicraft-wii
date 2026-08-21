#include "../utils/arraylist.h"

#include "particle/smashparticle.h"
#include "particle/textparticle.h"

#include "airwizard.h"
#include "anvil.h"
#include "chest.h"
#include "entity.h"
#include "furnace.h"
#include "furniture.h"
#include "inventory.h"
#include "itementity.h"
#include "lantern.h"
#include "oven.h"
#include "player.h"
#include "slime.h"
#include "spark.h"
#include "workbench.h"
#include "zombie.h"
#include "bed.h"
#include "skeleton.h"
#include "creeper.h"
#include "knight.h"
#include "cow.h"
#include "../item/item.h"

#include <string.h>
#include <stdio.h>

void call_entity_tick(Entity* entity) {
	switch (entity->type) {
		case PLAYER:
			player_tick((Player*) entity);
			break;
		case SMASHPARTICLE:
			smashparticle_tick((SmashParticle*) entity);
			break;
		case TEXTPARTICLE:
			textparticle_tick((TextParticle*) entity);
			break;
		case ITEMENTITY:
			itementity_tick((ItemEntity*) entity);
			break;
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
			furniture_tick((Furniture*)entity);
			break;
		case SLIME:
			slime_tick((Slime *) entity);
			break;
		case ZOMBIE:
			zombie_tick((Zombie *) entity);
			break;
		case SKELETON:
			skeleton_tick((Skeleton *) entity);
			break;
		case CREEPER:
			creeper_tick((Creeper *) entity);
			break;
		case KNIGHT:
			knight_tick((Knight *) entity);
			break;
		case COW:
			cow_tick((Cow *) entity);
			break;
		case AIRWIZARD:
			airwizard_tick((AirWizard *) entity);
			break;
		case SPARK:
			spark_tick((Spark *) entity);
			break;
		default:
			break;
	}
}

uint8_t call_entity_canSwim(Entity* entity) {
	switch (entity->type) {
		case PLAYER:
			return 1;
		default:
			return 0;
	}
}

uint8_t call_entity_isBlockableBy(Entity* entity, Mob* mob) {
	switch (entity->type) {
		case ITEMENTITY:
		case SPARK:
			return 0;
		case PLAYER:
		default:
			return 1;
	}
}

void call_entity_render(Entity* entity, Screen* screen) {
	switch (entity->type) {
		case PLAYER:
			player_render((Player*) entity, screen);
			break;
		case SMASHPARTICLE:
			smashparticle_render((SmashParticle*) entity, screen);
			break;
		case ITEMENTITY:
			itementity_render((ItemEntity*) entity, screen);
			break;
		case TEXTPARTICLE:
			textparticle_render((TextParticle*) entity, screen);
			break;
		case SLIME:
			slime_render((Slime *) entity, screen);
			break;
		case ZOMBIE:
			zombie_render((Zombie *) entity, screen);
			break;
		case SKELETON:
			skeleton_render((Skeleton *) entity, screen);
			break;
		case CREEPER:
			creeper_render((Creeper *) entity, screen);
			break;
		case KNIGHT:
			knight_render((Knight *) entity, screen);
			break;
		case COW:
			cow_render((Cow *) entity, screen);
			break;
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
			furniture_render((Furniture *) entity, screen);
			break;
		case AIRWIZARD:
			airwizard_render((AirWizard *) entity, screen);
			break;
		case SPARK:
			spark_render((Spark *) entity, screen);
			break;
		default:
			break;
	}
}

void call_entity_touchItem(Entity* entity, ItemEntity* item){
	Player* pl;
	switch (entity->type) {
		case PLAYER:
			pl = (Player*) entity;
			itementity_take(item, (Player *) entity);
			inventory_addItem(&pl->inventory, &item->item);
			break;
		default:
			break;
	}
}

void call_entity_die(Entity* entity) {
	switch (entity->type) {
		case PLAYER:
			player_die((Player *) entity);
			break;
		case SLIME:
			slime_die((Slime *) entity);
			break;
		case ZOMBIE:
			zombie_die((Zombie *) entity);
			break;
		case SKELETON:
			skeleton_die((Skeleton *) entity);
			break;
		case CREEPER:
			creeper_die((Creeper *) entity);
			break;
		case KNIGHT:
			knight_die((Knight *) entity);
			break;
		case COW:
			cow_die((Cow *) entity);
			break;
		case AIRWIZARD:
			airwizard_die((AirWizard *) entity);
			break;
		default:
			break;
	}
}

void call_entity_doHurt(Entity* entity, int damage, int attackDir) {
	switch (entity->type) {
		case PLAYER:
			player_doHurt((Player *) entity, damage, attackDir);
			break;
		case AIRWIZARD:
			airwizard_doHurt((AirWizard *) entity, damage, attackDir);
			break;
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
			mob_doHurt((Mob *) entity, damage, attackDir);
			break;
		default:
			break;
	}
}

void call_entity_hurtTile(Entity* entity, TileID tile, int x, int y, int damage) {
	switch (entity->type){
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
		case AIRWIZARD:
		case PLAYER:
			mob_hurtTile((Mob *) entity, tile, x, y, damage);
			break;
		default:
			break;
	}
}

void call_entity_hurt(Entity* entity, Mob* mob, int damage, int attackDir) {
	switch (entity->type) {
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
		case AIRWIZARD:
		case PLAYER:
			mob_hurt((Mob *) entity, mob, damage, attackDir);
			break;
		default:
			break;
	}
}

void call_entity_touchedBy(Entity* entity, Entity* e) {
	switch (entity->type) {
		case AIRWIZARD:
			airwizard_touchedBy((AirWizard *) entity, e);
			break;
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
			furniture_touchedBy((Furniture *) entity, e);
			break;
		case SLIME:
			slime_touchedBy((Slime *) entity, e);
			break;
		case ZOMBIE:
			zombie_touchedBy((Zombie *) entity, e);
			break;
		case SKELETON:
			skeleton_touchedBy((Skeleton *) entity, e);
			break;
		case CREEPER:
			creeper_touchedBy((Creeper *) entity, e);
			break;
		case KNIGHT:
			knight_touchedBy((Knight *) entity, e);
			break;
		case COW:
			cow_touchedBy((Cow *) entity, e);
			break;
		default:
			break;
	}
}

char call_entity_use(Entity* entity, Player* player, int attackDir) {
	switch (entity->type) {
		case ANVIL: return anvil_use((Anvil *) entity, player, attackDir);
		case BED: return bed_use((Bed *) entity, player, attackDir);
		case CHEST: return chest_use((Chest *) entity, player, attackDir);
		case FURNACE: return furnace_use((Furnace *) entity, player, attackDir);
		case OVEN: return oven_use((Oven *) entity, player, attackDir);
		case WORKBENCH: return workbench_use((Workbench *) entity, player, attackDir);
		default: return 0;
	}
}

char call_entity_blocks(Entity* entity, Entity* e) {
	switch (entity->type) {
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
			return 1;
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
		case AIRWIZARD:
		case PLAYER:
			return call_entity_isBlockableBy(e, (Mob *) entity);
		default:
			return 0;
	}
}

char entity_ismob(Entity* entity) {
	switch (entity->type) {
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
		case AIRWIZARD:
		case PLAYER:
			return 1;
		default:
			return 0;
	}
}

Furniture* entity_createFurniture(EntityId id) {
	Furniture* furn = 0;
	switch (id) {
		case ANVIL:
			furn = malloc(sizeof(Anvil));
			anvil_create((Anvil *) furn);
			break;
		case BED:
			furn = malloc(sizeof(Bed));
			bed_create((Bed *) furn);
			break;
		case CHEST:
			furn = malloc(sizeof(Chest));
			chest_create((Chest *) furn);
			break;
		case FURNACE:
			furn = malloc(sizeof(Furnace));
			furnace_create((Furnace *) furn);
			break;
		case LANTERN:
			furn = malloc(sizeof(Lantern));
			lantern_create((Lantern *) furn);
			break;
		case OVEN:
			furn = malloc(sizeof(Oven));
			oven_create((Oven *) furn);
			break;
		case WORKBENCH:
			furn = malloc(sizeof(Workbench));
			workbench_create((Workbench *) furn);
			break;
		default:
			break;
	}
	return furn;
}

char entity_isfurniture(Entity* entity) {
	switch (entity->type) {
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
			return 1;
		default:
			return 0;
	}
}

static inline int item_getLightRadius_safe(Item* item) {
	if (!item) return 0;
	if (item->id == FURNITURE && item->add.furniture.furniture) {
		if (item->add.furniture.furniture->entity.type == LANTERN) return 8;
	}
	return 0;
}

extern inline int call_entity_getLightRadius(Entity* entity) {
	int r, rr;
	Player* p;
	switch (entity->type) {
		case LANTERN:
			return 8;
		case PLAYER:
			p = (Player*) entity;
			r = 0;
			if (p->activeItem && item_getLightRadius_safe(p->activeItem) > 0) {
				r = item_getLightRadius_safe(p->activeItem);
			}
			if (p->inventory.items.size) {
				for (int i = 0; i < p->inventory.items.size; ++i) {
					Item* item = (Item*) p->inventory.items.elements[i];
					rr = item_getLightRadius_safe(item);
					if (rr > r) r = rr;
				}
			}
			return r;
		default:
			return 0;
	}
}

void call_entity_free(Entity* entity) {
	switch (entity->type) {
		case CHEST:
			chest_free((Chest *) entity);
			break;
		case PLAYER:
			player_free((Player *) entity);
			break;
		default:
			break;
	}
}

char call_entity_isSwimming(Entity* entity) {
	if (entity->level) {
		TileID tile = level_get_tile(entity->level, entity->x >> 4, entity->y >> 4);
		return tile == WATER || tile == LAVA;
	}
	return 0;
}

Furniture* furniture_create_copy(Furniture* old) {
	return entity_createFurniture(old->entity.type);
}
