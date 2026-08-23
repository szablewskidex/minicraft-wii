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
#include "chicken.h"
#include "pig.h"
#include "sheep.h"
#include "crab.h"
#include "frog.h"
#include "door.h"
#include "exporb.h"
#include "arrow.h"
#include "barrel.h"
#include "sign.h"
#include "bat.h"
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
		case EXPORB:
			exporb_tick((ExpOrb*) entity);
			break;
		case ARROW:
			arrow_tick((Arrow*) entity);
			break;
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
		case DOOR:
		case BARREL:
		case SIGN:
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
		case CHICKEN:
			chicken_tick((Chicken *) entity);
			break;
		case PIG:
			pig_tick((Pig *) entity);
			break;
		case SHEEP:
			sheep_tick((Sheep *) entity);
			break;
		case CRAB:
			crab_tick((Crab *) entity);
			break;
		case FROG:
			frog_tick((Frog *) entity);
			break;
		case BAT:
			bat_tick((Bat *) entity);
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
		case CRAB:
		case FROG:
		case BAT:
			return 1;
		default:
			return 0;
	}
}

uint8_t call_entity_isBlockableBy(Entity* entity, Mob* mob) {
	switch (entity->type) {
		case ITEMENTITY:
		case EXPORB:
		case ARROW:
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
		case EXPORB:
			exporb_render((ExpOrb*) entity, screen);
			break;
		case ARROW:
			arrow_render((Arrow*) entity, screen);
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
		case CHICKEN:
			chicken_render((Chicken *) entity, screen);
			break;
		case PIG:
			pig_render((Pig *) entity, screen);
			break;
		case SHEEP:
			sheep_render((Sheep *) entity, screen);
			break;
		case CRAB:
			crab_render((Crab *) entity, screen);
			break;
		case FROG:
			frog_render((Frog *) entity, screen);
			break;
		case BAT:
			bat_render((Bat *) entity, screen);
			break;
		case DOOR:
			door_render((Door *) entity, screen);
			break;
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
		case BARREL:
		case SIGN:
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
		case CHICKEN:
			chicken_die((Chicken *) entity);
			break;
		case PIG:
			pig_die((Pig *) entity);
			break;
		case SHEEP:
			sheep_die((Sheep *) entity);
			break;
		case CRAB:
			crab_die((Crab *) entity);
			break;
		case FROG:
			frog_die((Frog *) entity);
			break;
		case BAT:
			bat_die((Bat *) entity);
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
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
		case BAT:
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
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
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
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
		case BAT:
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
		case DOOR:
		case BARREL:
		case SIGN:
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
		case CHICKEN:
			chicken_touchedBy((Chicken *) entity, e);
			break;
		case PIG:
			pig_touchedBy((Pig *) entity, e);
			break;
		case SHEEP:
			sheep_touchedBy((Sheep *) entity, e);
			break;
		case CRAB:
			crab_touchedBy((Crab *) entity, e);
			break;
		case FROG:
			frog_touchedBy((Frog *) entity, e);
			break;
		case BAT:
			bat_touchedBy((Bat *) entity, e);
			break;
		case ITEMENTITY:
			if (e->type == PLAYER) {
				call_entity_touchItem(e, (ItemEntity *) entity);
			}
			break;
		case EXPORB:
			if (e->type == PLAYER) {
				exporb_touchedBy((ExpOrb*)entity, e);
			}
			break;
		case ARROW:
			arrow_touchedBy((Arrow*)entity, e);
			break;
		case PLAYER:
			if (e->type == ITEMENTITY) {
				call_entity_touchItem(entity, (ItemEntity *) e);
			} else if (e->type == EXPORB) {
				exporb_touchedBy((ExpOrb*)e, entity);
			} else if (e->type == ARROW) {
				arrow_touchedBy((Arrow*)e, entity);
			}
			break;
		default:
			break;
	}
}

#include "../sound.h"
#include "../item/resource/resource.h"
#include "../gfx/color.h"
#include <stdlib.h>
extern void input_rumble(int frames);
Mob* entity_createAnimal(EntityId id);

char animal_feedAndBreed(Entity* entity, Player* player) {
	if (!player || !player->activeItem || player->activeItem->id != RESOURCE) return 0;
	Resource* res = player->activeItem->add.resource.resource;

	int validFood = 0;
	if (entity->type == COW && res == &wheat) validFood = 1;
	if (entity->type == SHEEP && res == &wheat) validFood = 1;
	if (entity->type == PIG && (res == &carrot || res == &potato || res == &wheat || res == &apple)) validFood = 1;
	if (entity->type == CHICKEN && res == &seeds) validFood = 1;

	if (!validFood) return 0;

	int* loveTime = NULL;
	int* breedCooldown = NULL;
	if (entity->type == COW) {
		loveTime = &((Cow*)entity)->loveTime;
		breedCooldown = &((Cow*)entity)->breedCooldown;
	} else if (entity->type == PIG) {
		loveTime = &((Pig*)entity)->loveTime;
		breedCooldown = &((Pig*)entity)->breedCooldown;
	} else if (entity->type == SHEEP) {
		loveTime = &((Sheep*)entity)->loveTime;
		breedCooldown = &((Sheep*)entity)->breedCooldown;
	} else if (entity->type == CHICKEN) {
		loveTime = &((Chicken*)entity)->loveTime;
		breedCooldown = &((Chicken*)entity)->breedCooldown;
	}

	if (!loveTime || !breedCooldown) return 0;
	if (*loveTime > 0 || *breedCooldown > 0) return 0;

	// Pay 1 food item
	if (player->activeItem->add.resource.count > 1) {
		player->activeItem->add.resource.count--;
	} else {
		arraylist_removeElement(&player->inventory.items, player->activeItem);
		item_free(player->activeItem);
		free(player->activeItem);
		player->activeItem = 0;
	}

	*loveTime = 600; // 10 seconds of love mode

	TextParticle* tp = malloc(sizeof(TextParticle));
	if (tp) {
		textparticle_create(tp, "<3", entity->x, entity->y - 8, getColor4(-1, 500, 500, 555));
		level_addEntity(entity->level, &tp->entity);
	}

	sound_play(SND_CONFIRM);
	input_rumble(5);
	return 1;
}

void animal_tickBreeding(Entity* entity, int* loveTime, int* breedCooldown, int* xa, int* ya) {
	if (*breedCooldown > 0) {
		(*breedCooldown)--;
	}

	if (*loveTime > 0) {
		(*loveTime)--;

		if (*loveTime % 40 == 0) {
			TextParticle* tp = malloc(sizeof(TextParticle));
			if (tp) {
				textparticle_create(tp, "<3", entity->x, entity->y - 8, getColor4(-1, 500, 500, 555));
				level_addEntity(entity->level, &tp->entity);
			}
		}

		// Search nearby for same species partner in love within 80 pixels!
		ArrayList nearby;
		create_arraylist(&nearby);
		int r = 80;
		level_getEntities(entity->level, &nearby, entity->x - r, entity->y - r, entity->x + r, entity->y + r);

		Entity* partner = NULL;
		int bestDist = 999999;

		for (int i = 0; i < nearby.size; ++i) {
			Entity* other = nearby.elements[i];
			if (other == entity || other->type != entity->type) continue;

			int otherLove = 0;
			if (other->type == COW) otherLove = ((Cow*)other)->loveTime;
			else if (other->type == PIG) otherLove = ((Pig*)other)->loveTime;
			else if (other->type == SHEEP) otherLove = ((Sheep*)other)->loveTime;
			else if (other->type == CHICKEN) otherLove = ((Chicken*)other)->loveTime;

			if (otherLove > 0) {
				int dx = other->x - entity->x;
				int dy = other->y - entity->y;
				int d2 = dx * dx + dy * dy;
				if (d2 < bestDist) {
					bestDist = d2;
					partner = other;
				}
			}
		}

		arraylist_remove(&nearby);

		if (partner) {
			int dx = partner->x - entity->x;
			int dy = partner->y - entity->y;

			// Walk towards partner
			*xa = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
			*ya = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);

			// If close enough (within 16 pixels) -> BREED!
			if (abs(dx) <= 16 && abs(dy) <= 16) {
				*loveTime = 0;
				*breedCooldown = 1800; // 30s cooldown

				if (partner->type == COW) { ((Cow*)partner)->loveTime = 0; ((Cow*)partner)->breedCooldown = 1800; }
				else if (partner->type == PIG) { ((Pig*)partner)->loveTime = 0; ((Pig*)partner)->breedCooldown = 1800; }
				else if (partner->type == SHEEP) { ((Sheep*)partner)->loveTime = 0; ((Sheep*)partner)->breedCooldown = 1800; }
				else if (partner->type == CHICKEN) { ((Chicken*)partner)->loveTime = 0; ((Chicken*)partner)->breedCooldown = 1800; }

				// Spawn Baby!
				Mob* baby = entity_createAnimal(entity->type);
				if (baby) {
					baby->entity.x = (entity->x + partner->x) / 2;
					baby->entity.y = (entity->y + partner->y) / 2;
					if (baby->entity.type == CHICKEN) {
						((Chicken*)baby)->babyTime = 3600 * 2; // 2 minutes as baby chick
					}
					level_addEntity(entity->level, (Entity*)baby);
				}

				// Spawn Exp Orb
				ExpOrb* exp = malloc(sizeof(ExpOrb));
				if (exp) {
					exporb_create(exp, (entity->x + partner->x) / 2, (entity->y + partner->y) / 2, 5);
					level_addEntity(entity->level, (Entity*)exp);
				}

				// Love explosion!
				TextParticle* tp = malloc(sizeof(TextParticle));
				if (tp) {
					textparticle_create(tp, "<3 <3", (entity->x + partner->x) / 2, (entity->y + partner->y) / 2 - 12, getColor4(-1, 500, 500, 555));
					level_addEntity(entity->level, &tp->entity);
				}

				sound_play(SND_CONFIRM);
				input_rumble(8);
			}
		}
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
		case DOOR: return door_use((Door *) entity, player, attackDir);
		case BARREL: return barrel_use((Barrel *) entity, player, attackDir);
		case SIGN: return sign_use((Sign *) entity, player, attackDir);
		case COW:
		case PIG:
		case SHEEP:
		case CHICKEN:
			return animal_feedAndBreed(entity, player);
		default: return 0;
	}
}

char call_entity_blocks(Entity* entity, Entity* e) {
	switch (entity->type) {
		case DOOR: {
			Door* d = (Door*)entity;
			return !d->isOpen;
		}
		case ANVIL:
		case BED:
		case CHEST:
		case FURNACE:
		case LANTERN:
		case OVEN:
		case WORKBENCH:
		case BARREL:
		case SIGN:
			return 1;
		case SLIME:
		case ZOMBIE:
		case SKELETON:
		case CREEPER:
		case KNIGHT:
		case COW:
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
		case BAT:
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
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
		case BAT:
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
		case DOOR:
			furn = malloc(sizeof(Door));
			door_create((Door *) furn);
			break;
		case BARREL:
			furn = malloc(sizeof(Barrel));
			barrel_create((Barrel *) furn);
			break;
		case SIGN:
			furn = malloc(sizeof(Sign));
			sign_create((Sign *) furn);
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
		case DOOR:
		case BARREL:
		case SIGN:
			return 1;
		default:
			return 0;
	}
}

char entity_isAnimal(Entity* entity) {
	switch (entity->type) {
		case COW:
		case CHICKEN:
		case PIG:
		case SHEEP:
		case CRAB:
		case FROG:
			return 1;
		default:
			return 0;
	}
}

Mob* entity_createAnimal(EntityId id) {
	Mob* mob = NULL;
	switch (id) {
		case COW: {
			Cow* cow = malloc(sizeof(Cow));
			cow_create(cow);
			mob = (Mob*)cow;
			break;
		}
		case CHICKEN: {
			Chicken* chk = malloc(sizeof(Chicken));
			chicken_create(chk);
			mob = (Mob*)chk;
			break;
		}
		case PIG: {
			Pig* pig = malloc(sizeof(Pig));
			pig_create(pig);
			mob = (Mob*)pig;
			break;
		}
		case SHEEP: {
			Sheep* sheep = malloc(sizeof(Sheep));
			sheep_create(sheep);
			mob = (Mob*)sheep;
			break;
		}
		case CRAB: {
			Crab* crab = malloc(sizeof(Crab));
			crab_create(crab);
			mob = (Mob*)crab;
			break;
		}
		case FROG: {
			Frog* frog = malloc(sizeof(Frog));
			frog_create(frog);
			mob = (Mob*)frog;
			break;
		}
		case BAT: {
			Bat* bat = malloc(sizeof(Bat));
			bat_create(bat);
			mob = (Mob*)bat;
			break;
		}
		default:
			break;
	}
	return mob;
}

static inline int item_getLightRadius_safe(Item* item) {
	if (!item) return 0;
	if (item->id == FURNITURE && item->add.furniture.furniture) {
		if (item->add.furniture.furniture->entity.type == LANTERN) return 8;
	}
	if (item->id == RESOURCE && item->add.resource.resource == &torchItem) {
		return 6;
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
			r = 3;
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
