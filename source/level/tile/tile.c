#include "tile.h"
#include "grass_tile.h"
#include "water_tile.h"
#include "flower_tile.h"
#include "tree_tile.h"

#include "sand_tile.h"
#include "cactus_tile.h"
#include "hole_tile.h"
#include "sapling_tile.h"

#include "lava_tile.h"
#include "stairs_tile.h"
#include "hard_rock_tile.h"
#include "ore_tile.h"
#include "wheat_tile.h"
#include "rock_tile.h"
#include "dirt_tile.h"
#include "cloud_cactus_tile.h"
#include "cloud_tile.h"
#include "farmland.h"
#include "wood_floor_tile.h"
#include "wood_wall_tile.h"
#include "torch_tile.h"
#include "wildflower_tile.h"
#include "path_tile.h"
#include "stone_wall_tile.h"
#include "pot_tile.h"
#include "tombstone_tile.h"
#include "fence_tile.h"
#include "stone_floor_tile.h"
#include "glass_wall_tile.h"
#include "ladder_tile.h"

#include "../../gfx/screen.h"
#include "../../item/resource/resource.h"
#include "../level.h"

#include "../../entity/entity.h"
#include "../../entity/mob.h"
#include "../../entity/_entity_caller.h"

#include <string.h>

Tile tiles[256];
int tile_tickCount = 0;

void init_tiles(){

	for(int i = 0; i < 256; ++i){
		memset(tiles + i, 0, sizeof(Tile));
	}

	grasstile_init(GRASS);
	tile_init(ROCK);
	watertile_init(WATER);
	flowertile_init(FLOWER);
	treetile_init(TREE);
	treetile_init(BIRCH_TREE);
	treetile_init(SPRUCE_TREE);
	tile_init(DIRT);
	sandtile_init(SAND);
	cactustile_init(CACTUS);
	holetile_init(HOLE);
	saplingtile_init(TREE_SAPLING, GRASS, TREE);
	saplingtile_init(CACTUS_SAPLING, SAND, CACTUS);
	tile_init(FARMLAND);
	wheat_tile_init(WHEAT);
	lavatile_init(LAVA);
	stairstile_init(STAIRS_DOWN, 0);
	stairstile_init(STAIRS_UP, 1);
	tile_init(INFINITE_FALL);
	tile_init(CLOUD);
	tile_init(HARD_ROCK);
	oretile_init(IRON_ORE, &ironOre);
	oretile_init(GOLD_ORE, &goldOre);
	oretile_init(GEM_ORE, &gem);
	tile_init(CLOUD_CACTUS);
	woodfloortile_init(WOOD_FLOOR);
	woodwalltile_init(WOOD_WALL);
	wildflowertile_init(WILDFLOWER);
	pathtile_init(DIRT_PATH);
	torchtile_init(TORCH_TILE);
	stonewalltile_init(STONE_WALL);
	pottile_init(POT_TILE);
	tombstonetile_init(TOMBSTONE_TILE);
	fencetile_init(FENCE_TILE);
	stonefloortile_init(STONE_FLOOR);
	glasswalltile_init(GLASS_WALL);
	laddertile_init(LADDER_TILE);
}

void tile_init(TileID id){
	Tile* t = tiles + id;
	t->connectsToGrass = t->connectsToLava = t->connectsToSand = t->connectsToWater = 0;
	t->id = id;
}

void tile_render(TileID id, Screen* screen, Level* level, int x, int y){
	switch(id){
		case CACTUS:
			cactustile_render(id, screen, level, x, y);
			break;
		case CLOUD_CACTUS:
			cloudcactustile_render(id, screen, level, x, y);
			break;
		case CLOUD:
			cloudtile_render(id, screen, level, x, y);
			break;
		case DIRT:
			dirttile_render(id, screen, level, x, y);
			break;
		case FARMLAND:
			farmlandtile_render(id, screen, level, x, y);
			break;
		case FLOWER:
			flowertile_render(id, screen, level, x, y);
			break;
		case GRASS:
			grasstile_render(id, screen, level, x, y);
			break;
		case HARD_ROCK:
			hardrocktile_render(id, screen, level, x, y);
			break;
		case HOLE:
			holetile_render(id, screen, level, x, y);
			break;
		case LAVA:
			lavatile_render(id, screen, level, x, y);
			break;
		case IRON_ORE:
		case GOLD_ORE:
		case GEM_ORE:
			oretile_render(id, screen, level, x, y);
			break;
		case ROCK:
			rocktile_render(id, screen, level, x, y);
			break;
		case SAND:
			sandtile_render(id, screen, level, x, y);
			break;
		case TREE_SAPLING:
		case CACTUS_SAPLING:
			saplingtile_render(id, screen, level, x, y);
			break;
		case STAIRS_UP:
		case STAIRS_DOWN:
			stairstile_render(id, screen, level, x, y);
			break;
		//case STONE: XXX unused?
		//
		//	break;
		case TREE:
		case BIRCH_TREE:
		case SPRUCE_TREE:
			treetile_render(id, screen, level, x, y);
			break;
		case WOOD_FLOOR:
			woodfloortile_render(id, screen, level, x, y);
			break;
		case WOOD_WALL:
			woodwalltile_render(id, screen, level, x, y);
			break;
		case WILDFLOWER:
			wildflowertile_render(id, screen, level, x, y);
			break;
		case DIRT_PATH:
			pathtile_render(id, screen, level, x, y);
			break;
		case TORCH_TILE:
			torchtile_render(id, screen, level, x, y);
			break;
		case STONE_WALL:
			stonewalltile_render(id, screen, level, x, y);
			break;
		case POT_TILE:
			pottile_render(id, screen, level, x, y);
			break;
		case TOMBSTONE_TILE:
			tombstonetile_render(id, screen, level, x, y);
			break;
		case FENCE_TILE:
			fencetile_render(id, screen, level, x, y);
			break;
		case WATER:
			watertile_render(id, screen, level, x, y);
			break;
		case WHEAT:
			wheattile_render(id, screen, level, x, y);
			break;
		case STONE_FLOOR:
			stonefloortile_render(id, screen, level, x, y);
			break;
		case GLASS_WALL:
			glasswalltile_render(id, screen, level, x, y);
			break;
		case LADDER_TILE:
			laddertile_render(id, screen, level, x, y);
			break;
		default:
		case INFINITE_FALL:
			//render nothing
			break;
	}
}

char tile_mayPass(TileID id, Level* level, int x, int y, Entity* e){

    // -DGODMODE
    #ifdef GODMODE
	    if (e->type == PLAYER) {
            return 1;
        }
    #endif

	switch (id) {
		case CACTUS:
		case HARD_ROCK:
		case GEM_ORE:
		case GOLD_ORE:
		case IRON_ORE:
		case ROCK:
		// XXX unused case STONE:
		case TREE:
		case BIRCH_TREE:
		case SPRUCE_TREE:
		case WOOD_WALL:
		case STONE_WALL:
		case GLASS_WALL:
		case POT_TILE:
		case TOMBSTONE_TILE:
		case FENCE_TILE:
			return 0;
		case CLOUD_CACTUS:
		case INFINITE_FALL:
			if (e->type == AIRWIZARD) {
                return 1;
            }
			return 0;
		case HOLE:
		case LAVA:
		case WATER:
			return call_entity_canSwim(e);
		case WOOD_FLOOR:
		case STONE_FLOOR:
		case LADDER_TILE:
		case CLOUD:
		default:
			return 1;
	}
}


int tile_getLightRadius(TileID id, Level* level, int x, int y){
	switch(id){
		case LAVA:
			return 6;
		case TORCH_TILE:
			return 5;
		default:
			return 0;
	}
}


#include "../../sound.h"

void tile_hurt(TileID id, Level* level, int x, int y, Mob* source, int dmg, int attackDir){
	switch(id){
		case CACTUS:
			cactus_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case CLOUD_CACTUS:
			cloudcactustile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case FLOWER:
			flowertile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case HARD_ROCK:
			hardrocktile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case GEM_ORE:
		case GOLD_ORE:
		case IRON_ORE:
			oretile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case ROCK:
			rocktile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case CACTUS_SAPLING:
		case TREE_SAPLING:
			saplingtile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case TREE:
		case BIRCH_TREE:
		case SPRUCE_TREE:
			treetile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case WOOD_WALL:
			woodwalltile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case WILDFLOWER:
			wildflowertile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case DIRT_PATH:
			pathtile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case TORCH_TILE:
			torchtile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case STONE_WALL:
			stonewalltile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case POT_TILE:
			pottile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case TOMBSTONE_TILE:
			tombstonetile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case FENCE_TILE:
			fencetile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case STONE_FLOOR:
			stonefloortile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case GLASS_WALL:
			glasswalltile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case LADDER_TILE:
			laddertile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		case WHEAT:
			wheattile_hurt(id, level, x, y, source, dmg, attackDir);
			break;
		default:
			break;
	}

	//TODO
}

void tile_bumpedInto(TileID id, Level* level, int x, int y, Entity* entity){
	switch(id){
		case CACTUS:
			call_entity_hurtTile(entity, id, x, y, 1);
			break;
		case CLOUD_CACTUS:
			if(entity->type == AIRWIZARD) break;
			call_entity_hurtTile(entity, id, x, y, 3);
			break;
		case GEM_ORE:
		case GOLD_ORE:
		case IRON_ORE:
			call_entity_hurtTile(entity, id, x, y, 3);
			break;
	}
}

void tile_tick(TileID id, Level* level, int xt, int yt) {
	switch(id){
		case CACTUS:
			cactustile_tick(id, level, xt, yt);
			break;
		case FARMLAND:
			farmland_tick(id, level, xt, yt);
			break;
		case GRASS:
			grasstile_tick(id, level, xt, yt);
			break;
		case HARD_ROCK:
			hardrocktile_tick(id, level, xt, yt);
			break;
		case LAVA:
			lavatile_tick(id, level, xt, yt);
			break;
		case ROCK:
			rocktile_tick(id, level, xt, yt);
			break;
		case SAND:
			sandtile_tick(id, level, xt, yt);
			break;
		case TREE_SAPLING:
		case CACTUS_SAPLING:
			saplingtile_tick(id, level, xt, yt);
			break;
		case TREE:
		case BIRCH_TREE:
		case SPRUCE_TREE:
			treetile_tick(id, level, xt, yt);
			break;
		case WATER:
			watertile_tick(id, level, xt, yt);
			break;
		case WHEAT:
			wheattile_tick(id, level, xt, yt);
			break;
		case INFINITE_FALL:
		default:
			//do nothing
			break;
	}
}

void tile_steppedOn(TileID id, Level* level, int x, int y, Entity* entity){
	switch(id){
		case SAND:
			if(entity_ismob(entity)) level_set_data(level, x, y, 120);
			break;
		case FARMLAND:
			if(random_next_int(&tiles[id].random, 60) != 0) return;
			if(level_get_data(level, x, y) < 5) return;
			level_set_tile(level, x, y, DIRT, 0);
			break;
		case WHEAT:
			if(random_next_int(&tiles[id].random, 60) != 0) return;
			if(level_get_data(level, x, y) < 2) return;
			wheattile_harvest(id, level, x, y);
			break;
	}
}

char tile_interact(TileID id, Level* level, int xt, int yt, struct _Player* player, struct _Item* item, int attackDir){
	switch(id){
		case CLOUD_CACTUS:
			return cloudcactustile_interact(id, level, xt, yt, player, item, attackDir);
		case CLOUD:
			return cloudtile_interact(id, level, xt, yt, player, item, attackDir);
		case DIRT:
			return dirttile_interact(id, level, xt, yt, player, item, attackDir);
		case FARMLAND:
			return farmtile_interact(id, level, xt, yt, player, item, attackDir);
		case GRASS:
			return grasstile_interact(id, level, xt, yt, player, item, attackDir);
		case HARD_ROCK:
			return hardrocktile_interact(id, level, xt, yt, player, item, attackDir);
		case GEM_ORE:
		case GOLD_ORE:
		case IRON_ORE:
			return oretile_interact(id, level, xt, yt, player, item, attackDir);
		case ROCK:
			return rocktile_interact(id, level, xt, yt, player, item, attackDir);
		case SAND:
			return sandtile_interact(id, level, xt, yt, player, item, attackDir);
		case TREE:
		case BIRCH_TREE:
		case SPRUCE_TREE:
			return treetile_interact(id, level, xt, yt, player, item, attackDir);
		case WOOD_FLOOR:
			return woodfloortile_interact(id, level, xt, yt, player, item, attackDir);
		case WOOD_WALL:
			return woodwalltile_interact(id, level, xt, yt, player, item, attackDir);
		case STONE_WALL:
			return stonewalltile_interact(id, level, xt, yt, player, item, attackDir);
		case POT_TILE:
			return pottile_interact(id, level, xt, yt, player, item, attackDir);
		case TOMBSTONE_TILE:
			return tombstonetile_interact(id, level, xt, yt, player, item, attackDir);
		case FENCE_TILE:
			return fencetile_interact(id, level, xt, yt, player, item, attackDir);
		case WHEAT:
			return wheattile_interact(id, level, xt, yt, player, item, attackDir);
		case FLOWER:
			return flowertile_interact(id, level, xt, yt, player, item, attackDir);
		case DIRT_PATH:
			return pathtile_interact(id, level, xt, yt, player, item, attackDir);
		case TORCH_TILE:
			return torchtile_interact(id, level, xt, yt, player, item, attackDir);
		case STONE_FLOOR:
			return stonefloortile_interact(id, level, xt, yt, player, item, attackDir);
		case GLASS_WALL:
			return glasswalltile_interact(id, level, xt, yt, player, item, attackDir);
		case LADDER_TILE:
			return laddertile_interact(id, level, xt, yt, player, item, attackDir);
		default:
			return 0;
	}
}

char tile_use(TileID id, Level* level, int xt, int yt, Player* player, int attackDir){
	//doesnt do anything
	return 0;
}

char tile_connectsToLiquid(TileID id) {
	return tiles[id].connectsToWater || tiles[id].connectsToLava;
}
