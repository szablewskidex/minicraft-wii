#include "level.h"
#include "levelgen/levelgen.h"
#include "tile/tile.h"

#include "../entity/_entity_caller.h"
#include "../entity/slime.h"
#include "../entity/zombie.h"
#include "../entity/airwizard.h"
#include "../entity/chest.h"
#include "../item/resourceitem.h"
#include "../item/resource/resource.h"
#include "../utils/javarandom.h"
#include "../utils/utils.h"
#include "../gfx/screen.h"
#include "../utils/arraylist.h"


void level_init(Level* lvl, int w, int h, int level, Level* parent) {
	random_set_seed(&lvl->random, getTimeMS());
	create_arraylist(&lvl->entities);

	lvl->dirtColor = level < 0 ? 222 : 322;
	lvl->grassColor = 141;
	lvl->sandColor = 550;
	lvl->monsterDensity = 8;

	lvl->depth = level;
	lvl->w = w;
	lvl->h = h;

	if (level == 0) {
		createAndValidateTopMap(&lvl->tiles, &lvl->data, w, h);
	} else if(level < 0) {
		createAndValidateUndergroundMap(&lvl->tiles, &lvl->data, w, h, -level);
		lvl->monsterDensity = 4;
	} else {
		createAndValidateSkyMap(&lvl->tiles, &lvl->data, w, h);
		lvl->monsterDensity = 4;
	}

	if (parent) {
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				if (level_get_tile(parent, x, y) == STAIRS_DOWN) {
					level_set_tile(lvl, x, y, STAIRS_UP, 0);

					if (level == 0) {
						level_set_tile(lvl, x - 1, y, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x + 1, y, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x, y - 1, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x, y + 1, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x - 1, y - 1, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x - 1, y + 1, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x + 1, y - 1, (int) HARD_ROCK, 0);
						level_set_tile(lvl, x + 1, y + 1, (int) HARD_ROCK, 0);
					}else{
						level_set_tile(lvl, x - 1, y, (int) DIRT, 0);
						level_set_tile(lvl, x + 1, y, (int) DIRT, 0);
						level_set_tile(lvl, x, y - 1, (int) DIRT, 0);
						level_set_tile(lvl, x, y + 1, (int) DIRT, 0);
						level_set_tile(lvl, x - 1, y - 1, (int) DIRT, 0);
						level_set_tile(lvl, x - 1, y + 1, (int) DIRT, 0);
						level_set_tile(lvl, x + 1, y - 1, (int) DIRT, 0);
						level_set_tile(lvl, x + 1, y + 1, (int) DIRT, 0);
					}
				}
			}
		}
	}

	lvl->entitiesInTiles = malloc(sizeof(ArrayList) * w * h);
	for (int i = 0; i < w * h; ++i) {
		create_arraylist(lvl->entitiesInTiles + i);
	}

	if (level < 0) {
		// Generate underground dungeon rooms with stone walls, pots, torches and treasure chests!
		int numDungeons = (w >= 128) ? 4 : 2;
		for (int d = 0; d < numDungeons; ++d) {
			int rx = 12 + (d * (w / (numDungeons + 1))) + random_next_int(&lvl->random, 10);
			int ry = 12 + random_next_int(&lvl->random, h - 30);
			if (rx + 5 < w && ry + 5 < h) {
				for (int dy = 0; dy < 5; ++dy) {
					for (int dx = 0; dx < 5; ++dx) {
						if (dx == 0 || dx == 4 || dy == 0 || dy == 4) {
							level_set_tile(lvl, rx + dx, ry + dy, (int)STONE_WALL, 0);
						} else {
							level_set_tile(lvl, rx + dx, ry + dy, (int)DIRT, 0);
						}
					}
				}
				// Entry opening
				level_set_tile(lvl, rx + 2, ry, (int)DIRT, 0);

				// Place torches inside dungeon room top corners
				level_set_tile(lvl, rx + 1, ry + 1, (int)TORCH_TILE, 0);
				level_set_tile(lvl, rx + 3, ry + 1, (int)TORCH_TILE, 0);

				// Place smashable clay pots in bottom corners!
				level_set_tile(lvl, rx + 1, ry + 3, (int)POT_TILE, 0);
				level_set_tile(lvl, rx + 3, ry + 3, (int)POT_TILE, 0);

				// Place treasure Chest in center
				Chest* chest = malloc(sizeof(Chest));
				if (chest) {
					chest_create(chest);
					chest->furniture.entity.x = (rx + 2) * 16 + 8;
					chest->furniture.entity.y = (ry + 2) * 16 + 8;

					Item loot1, loot2, loot3, loot4;
					if (level == -1) {
						// Upper Cave: Iron, Torches, Arrows, Steak
						resourceitem_create_cnt(&loot1, &ironIngot, 4 + random_next_int(&lvl->random, 4));
						inventory_addItem(&chest->inventory, &loot1);
						resourceitem_create_cnt(&loot2, &torchItem, 8);
						inventory_addItem(&chest->inventory, &loot2);
						resourceitem_create_cnt(&loot3, &arrow, 15);
						inventory_addItem(&chest->inventory, &loot3);
						resourceitem_create_cnt(&loot4, &cookedSteak, 5);
						inventory_addItem(&chest->inventory, &loot4);
					} else if (level == -2) {
						// Deep Cave: Gold, Gems, Bow, Torches
						resourceitem_create_cnt(&loot1, &goldIngot, 5 + random_next_int(&lvl->random, 5));
						inventory_addItem(&chest->inventory, &loot1);
						resourceitem_create_cnt(&loot2, &gem, 3 + random_next_int(&lvl->random, 3));
						inventory_addItem(&chest->inventory, &loot2);
						resourceitem_create(&loot3, &bow);
						inventory_addItem(&chest->inventory, &loot3);
						resourceitem_create_cnt(&loot4, &torchItem, 12);
						inventory_addItem(&chest->inventory, &loot4);
					} else {
						// Dungeon / Boss Cave: Gems, Gold, Steak, Torches
						resourceitem_create_cnt(&loot1, &gem, 6 + random_next_int(&lvl->random, 6));
						inventory_addItem(&chest->inventory, &loot1);
						resourceitem_create_cnt(&loot2, &goldIngot, 8);
						inventory_addItem(&chest->inventory, &loot2);
						resourceitem_create_cnt(&loot3, &cookedSteak, 8);
						inventory_addItem(&chest->inventory, &loot3);
						resourceitem_create_cnt(&loot4, &torchItem, 16);
						inventory_addItem(&chest->inventory, &loot4);
					}

					level_addEntity(lvl, (Entity*)chest);
				}
			}
		}
	}

	if (level == 0) {
		// Generate 2 small atmospheric Graveyards on surface
		for (int g = 0; g < 2; ++g) {
			int gx = 16 + random_next_int(&lvl->random, w - 40);
			int gy = 16 + random_next_int(&lvl->random, h - 40);
			if (level_get_tile(lvl, gx, gy) == GRASS) {
				level_set_tile(lvl, gx, gy, (int)TOMBSTONE_TILE, 0);
				level_set_tile(lvl, gx + 2, gy, (int)TOMBSTONE_TILE, 0);
				level_set_tile(lvl, gx + 1, gy + 1, (int)TOMBSTONE_TILE, 0);
				level_set_tile(lvl, gx - 1, gy, (int)FENCE_TILE, 0);
				level_set_tile(lvl, gx + 3, gy, (int)FENCE_TILE, 0);
				level_set_tile(lvl, gx, gy - 1, (int)FENCE_TILE, 0);
				level_set_tile(lvl, gx + 2, gy - 1, (int)FENCE_TILE, 0);
			}
		}
	}

	if (level == 1) {
		AirWizard* wizard = malloc(sizeof(AirWizard));
		airwizard_create(wizard);
		wizard->mob.entity.x = w * 8;
		wizard->mob.entity.y = h * 8;
		level_addEntity(lvl, &wizard->mob.entity);
	}
}


void level_renderBackground(Level* level, Screen* screen, int xScroll, int yScroll) {
	int xo = xScroll >> 4;
	int yo = yScroll >> 4;

	int w = (screen->w + 15) >> 4;
	int h = (screen->h + 15) >> 4;

	screen_set_offset(screen, xScroll, yScroll);

	for (int y = yo; y <= h + yo; ++y) {
		for (int x = xo; x <= w + xo; ++x) {
			unsigned char tile = level_get_tile(level, x, y);
			tile_render(tile, screen, level, x, y);
		}
	}

	screen_set_offset(screen, 0, 0);
}


int _cmpEnt(const void* ent, const void* ent2) {
	Entity* e = *(Entity**) ent;
	Entity* e2 = *(Entity**) ent2;

	if (e2->y < e->y) return 1;
	if (e2->y > e->y) return -1;
	return 0;
}


void level_sortAndRender(Level* level, Screen* screen, ArrayList* list) {
	qsort(list->elements, list->size, sizeof(*list->elements), _cmpEnt);
	for (int i = 0; i < list->size; ++i) {
		call_entity_render((Entity*) list->elements[i], screen);
	}
}


#include "../entity/skeleton.h"
#include "../entity/creeper.h"
#include "../entity/knight.h"
#include "../entity/cow.h"
#include "../entity/chicken.h"
#include "../entity/pig.h"
#include "../entity/sheep.h"
#include "../entity/crab.h"
#include "../entity/frog.h"
#include "../game.h"

static int countLivingEntities(Level* level, int hostileOnly) {
	int count = 0;
	for (int i = 0; i < level->entities.size; ++i) {
		Entity* e = (Entity*)level->entities.elements[i];
		if (!e || e->removed) continue;
		if (e->type == PLAYER || e->type == ITEMENTITY || e->type == EXPORB || e->type == ARROW) continue;
		if (e->type == CHEST || e->type == WORKBENCH || e->type == FURNACE || e->type == OVEN || e->type == ANVIL || e->type == BED || e->type == LANTERN || e->type == DOOR) continue;

		int isHostile = (e->type == ZOMBIE || e->type == SKELETON || e->type == CREEPER || e->type == KNIGHT || e->type == SLIME || e->type == AIRWIZARD);
		if (hostileOnly == 1 && isHostile) count++;
		else if (hostileOnly == 0 && !isHostile) count++;
		else if (hostileOnly == -1) count++;
	}
	return count;
}

void level_trySpawn(Level* level, int count){
	Random* random = &level->random;

	for (int i = 0; i < count; ++i) {
		Mob* mob = 0;

		int minLevel = 1;
		int maxLevel = 1;

		if (level->depth < 0) maxLevel = (-level->depth) + 1;
		if (level->depth > 0) minLevel = maxLevel = 4;

		int lvl = random_next_int(random, maxLevel - minLevel + 1) + minLevel;
		int choice = random_next_int(random, 100);

		if (level->depth == 0) {
			// Surface level: check Day / Night
			int dayTime = game_gameTime % 24000;
			int isNight = (dayTime > 12000 && dayTime < 22000);

			if (!isNight) {
				// Day: Cap passive animals at 20 max!
				int passiveCount = countLivingEntities(level, 0);
				if (passiveCount >= 20) return;

				if (choice < 20) {
					mob = malloc(sizeof(Cow));
					cow_create((Cow *) mob);
				} else if (choice < 40) {
					mob = malloc(sizeof(Chicken));
					chicken_create((Chicken *) mob);
				} else if (choice < 60) {
					mob = malloc(sizeof(Pig));
					pig_create((Pig *) mob);
				} else if (choice < 80) {
					mob = malloc(sizeof(Sheep));
					sheep_create((Sheep *) mob);
				} else if (choice < 90) {
					mob = malloc(sizeof(Crab));
					crab_create((Crab *) mob);
				} else {
					mob = malloc(sizeof(Frog));
					frog_create((Frog *) mob);
				}
			} else {
				// Night: Cap hostile night monsters at 18 max!
				int monsterCount = countLivingEntities(level, 1);
				if (monsterCount >= 18) return;

				if (choice < 35) {
					mob = malloc(sizeof(Zombie));
					zombie_create((Zombie *) mob, lvl);
				} else if (choice < 65) {
					mob = malloc(sizeof(Skeleton));
					skeleton_create((Skeleton *) mob, lvl);
				} else if (choice < 85) {
					mob = malloc(sizeof(Creeper));
					creeper_create((Creeper *) mob, lvl);
				} else {
					mob = malloc(sizeof(Slime));
					slime_create((Slime *) mob, lvl);
				}
			}
		} else if (level->depth < 0) {
			// Underground Caves: Cap monsters at 20 max!
			int caveMonsterCount = countLivingEntities(level, 1);
			if (caveMonsterCount >= 20) return;

			if (choice < 25) {
				mob = malloc(sizeof(Slime));
				slime_create((Slime *) mob, lvl);
			} else if (choice < 50) {
				mob = malloc(sizeof(Zombie));
				zombie_create((Zombie *) mob, lvl);
			} else if (choice < 70) {
				mob = malloc(sizeof(Skeleton));
				skeleton_create((Skeleton *) mob, lvl);
			} else if (choice < 85) {
				mob = malloc(sizeof(Creeper));
				creeper_create((Creeper *) mob, lvl);
			} else {
				mob = malloc(sizeof(Knight));
				knight_create((Knight *) mob, lvl);
			}
		} else {
			int skyMonsterCount = countLivingEntities(level, 1);
			if (skyMonsterCount >= 12) return;

			mob = malloc(sizeof(Slime));
			slime_create((Slime *) mob, lvl);
		}

		if (!mob) continue;

		if (mob_findStartPos(mob, level)) {
			level_addEntity(level, &mob->entity);
		} else {
			call_entity_free(&mob->entity);
			free(mob);
		}
	}
}


void level_renderSprites(Level* level, Screen* screen, int xScroll, int yScroll) {
	ArrayList rowSprites;

	create_arraylist(&rowSprites);

	int xo = xScroll >> 4;
	int yo = yScroll >> 4;

	int w = (screen->w + 15) >> 4;
	int h = (screen->h + 15) >> 4;

	screen_set_offset(screen, xScroll, yScroll);

	for (int y = yo; y < h+yo; ++y) {
		for (int x = xo; x <= w+xo; ++x) {

			if (x < 0 || y < 0 || x >= level->w || y >= level->h) continue;
			ArrayList* ents = &level->entitiesInTiles[x + y * level->w];

            // TODO: use memcpy?
			for (int i = 0; i < ents->size; ++i) {
                arraylist_push(&rowSprites, ents->elements[i]);
            }
		}

		if (rowSprites.size > 0) {
			level_sortAndRender(level, screen, &rowSprites);
		}

		arraylist_clear(&rowSprites);
	}

	arraylist_remove(&rowSprites);
	screen_set_offset(screen, 0, 0);
}



void renderLight(Level* level, Screen* screen, int xScroll, int yScroll) {
	int xo = xScroll >> 4;
	int yo = yScroll >> 4;
	int w = (screen->w + 15) >> 4;
	int h = (screen->h + 15) >> 4;

	screen_set_offset(screen, xScroll, yScroll);

	int r = 4;
	for (int y = yo - r; y <= h + yo + r; ++y) {
		for (int x = xo - r; x <= w + xo + r; ++x) {
			if (x < 0 || y < 0 || x >= level->w || y >= level->h) continue;

			ArrayList* entities = &level->entitiesInTiles[x + y * level->w];

			for (int i = 0; i < entities->size; ++i) {
				Entity* e = entities->elements[i];
				int lr = call_entity_getLightRadius(e);
				if(lr > 0) screen_render_light(screen, e->x - 1, e->y - 4, lr * 8);
			}

			TileID tile = level_get_tile(level, x, y);
			int lr = tile_getLightRadius(tile, level, x, y);
			if (lr > 0) screen_render_light(screen, x * 16 + 8, y * 16 + 8, lr * 8);
		}
	}

	screen_set_offset(screen, 0, 0);
}


extern inline unsigned char level_get_tile(Level* level, int x, int y){
	if(x < 0 || y < 0 || x >= level->w || y >= level->h) return ROCK;
	return level->tiles[x + y*level->w];
}


void level_set_tile(Level* level, int x, int y, int id, int data){
	if(x < 0 || y < 0 || x >= level->w || y >= level->h) return;
	level->tiles[x + y*level->w] = id;
	level->data[x + y*level->w] = data;
}


extern inline unsigned char level_get_data(Level* level, int x, int y){
	if(x < 0 || y < 0 || x >= level->w || y >= level->h) return 0;
	return level->data[x + y*level->w];
}


void level_set_data(Level* level, int x, int y, int val){
	if(x < 0 || y < 0 || x >= level->w || y >= level->h) return;
	level->data[x + y*level->w] = val;
}


void level_insertEntity(Level* level, int x, int y, Entity* entity){
	if(x < 0 || y < 0 || x >= level->w || y >= level->h) return;
	arraylist_push(&level->entitiesInTiles[x+y*level->w], entity);
}


void level_addEntity(Level* level, Entity* entity){
	// TODO: if(e is player) level->player = e;

	entity->removed = 0;
	arraylist_push(&level->entities, entity);
	entity_init(entity, level);

	level_insertEntity(level, entity->x >> 4, entity->y >> 4, entity);
}


void level_removeEntity1(Level* level, Entity* e) {
	arraylist_removeElement(&level->entities, e);
	int xto = e->x >> 4;
	int yto = e->y >> 4;
	level_removeEntity(level, xto, yto, e);
}


void level_removeEntity(Level* level, int x, int y, Entity* entity) {
	if (x < 0 || y < 0 || x >= level->w || y >= level->h) return;
	arraylist_removeElement(&level->entitiesInTiles[x+y*level->w], entity);
}


void level_getEntities(Level* level, ArrayList* list, int x0, int y0, int x1, int y1) {
	int xt0 = (x0 >> 4) - 1;
	int yt0 = (y0 >> 4) - 1;
	int xt1 = (x1 >> 4) + 1;
	int yt1 = (y1 >> 4) + 1;

	for (int y = yt0; y <= yt1; ++y) {
		for (int x = xt0; x <= xt1; ++x) {
			if (x < 0 || y < 0 || x >= level->w || y >= level->h) continue;

			ArrayList* entities = &level->entitiesInTiles[x + y *level->w];
			for (int i = 0; i < entities->size; ++i) {
				Entity* e = entities->elements[i];
				if (entity_intersects(e, x0, y0, x1, y1)) {
					arraylist_push(list, e);
				}
			}
		}
	}
}


void level_tick(Level* level) {
	if (random_next_int(&level->random, 60) == 0) {
		level_trySpawn(level, 1);
	}

	for (int i = 0; i < level->w*level->h / 50; ++i) {
		int xt = random_next_int(&level->random, level->w);
		int yt = random_next_int(&level->random, level->w);

		tile_tick(level_get_tile(level, xt, yt), level, xt, yt);
	}

	for (int i = 0; i < level->entities.size; ++i) {
		Entity* e = (Entity*) arraylist_get(&level->entities, i);
		int xto = e->x >> 4;
		int yto = e->y >> 4;

		call_entity_tick(e);

		if (e->removed) {
			arraylist_removeId(&level->entities, i--);
			level_removeEntity(level, xto, yto, e);

			if (e->type != PLAYER) {
				call_entity_free((Entity*) e);
				free(e);
			}

		} else {
			int xt = e->x >> 4;
			int yt = e->y >> 4;

			if (xto != xt || yto != yt) {
				level_removeEntity(level, xto, yto, e);
				level_insertEntity(level, xt, yt, e);
			}
		}
	}
}


void level_free(Level* lvl) {
	if (lvl->tiles) free(lvl->tiles);
	if (lvl->data) free(lvl->data);

	if (lvl->entities.elements) {
		arraylist_remove(&lvl->entities);
	}

	if (lvl->entitiesInTiles) {
		for (int i = 0; i < lvl->w*lvl->h; ++i) {
			ArrayList* list = lvl->entitiesInTiles + i;

			for (int i = 0; i < list->size; ++i) {
				Entity* e = list->elements[i];

				if (e->type != PLAYER) {
					call_entity_free((Entity*) e);
					free(e);
				}
			}

			arraylist_remove(list);
		}

		free(lvl->entitiesInTiles);
	}
}
