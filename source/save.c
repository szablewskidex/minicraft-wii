#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item/item.h"
#include "item/resource/resource.h"
#include "item/resourceitem.h"
#include "item/tool_item.h"
#include "item/furniture_item.h"
#include "item/powergloveitem.h"

#include "entity/workbench.h"
#include "entity/furnace.h"
#include "entity/oven.h"
#include "entity/anvil.h"
#include "entity/chest.h"
#include "entity/lantern.h"
#include "entity/_entity_caller.h"

#define SAVE_MAGIC 0x4D435749 // "MCWI"
#define SAVE_VERSION 1

static Resource* resource_list[] = {
    &wood, &stone, &flower, &acorn, &dirt, &sand, &cactusFlower, &seeds,
    &wheat, &bread, &apple, &coal, &ironOre, &goldOre, &ironIngot, &goldIngot,
    &slime, &glass, &cloth, &cloud, &gem
};
#define NUM_RESOURCES (sizeof(resource_list) / sizeof(Resource*))

static int get_resource_index(Resource* res) {
    for (size_t i = 0; i < NUM_RESOURCES; ++i) {
        if (resource_list[i] == res) return (int)i;
    }
    return -1;
}

const char* get_save_path(void) {
#ifdef __wii__
    return "sd:/apps/minicraft/save.dat";
#else
    return "save.dat";
#endif
}

int save_exists(const char* filepath) {
    if (!filepath) filepath = get_save_path();
    FILE* f = fopen(filepath, "rb");
    if (!f) return 0;

    uint32_t magic = 0, version = 0;
    if (fread(&magic, sizeof(uint32_t), 1, f) != 1 ||
        fread(&version, sizeof(uint32_t), 1, f) != 1) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return (magic == SAVE_MAGIC && version == SAVE_VERSION);
}

int save_game(const char* filepath) {
    if (!filepath) filepath = get_save_path();
    if (!game_player || !isingame) return 0;

    FILE* f = fopen(filepath, "wb");
    if (!f) {
        // Fallback to local save.dat if sd: path failed
        f = fopen("save.dat", "wb");
        if (!f) return 0;
    }

    // 1. Header
    uint32_t magic = SAVE_MAGIC;
    uint32_t version = SAVE_VERSION;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    fwrite(&version, sizeof(uint32_t), 1, f);

    // 2. Game state
    int32_t gTime = game_gameTime;
    int32_t cLevel = game_currentLevel;
    int32_t hWon = game_hasWon;
    int32_t wTimer = game_wonTimer;
    fwrite(&gTime, sizeof(int32_t), 1, f);
    fwrite(&cLevel, sizeof(int32_t), 1, f);
    fwrite(&hWon, sizeof(int32_t), 1, f);
    fwrite(&wTimer, sizeof(int32_t), 1, f);

    // 3. Player state
    int32_t px = game_player->mob.entity.x;
    int32_t py = game_player->mob.entity.y;
    int32_t phealth = game_player->mob.health;
    int32_t pmaxHealth = game_player->mob.maxHealth;
    int32_t pdir = game_player->mob.dir;
    int32_t pstamina = game_player->stamina;
    int32_t pmaxStamina = game_player->maxStamina;
    int32_t pscore = game_player->score;

    fwrite(&px, sizeof(int32_t), 1, f);
    fwrite(&py, sizeof(int32_t), 1, f);
    fwrite(&phealth, sizeof(int32_t), 1, f);
    fwrite(&pmaxHealth, sizeof(int32_t), 1, f);
    fwrite(&pdir, sizeof(int32_t), 1, f);
    fwrite(&pstamina, sizeof(int32_t), 1, f);
    fwrite(&pmaxStamina, sizeof(int32_t), 1, f);
    fwrite(&pscore, sizeof(int32_t), 1, f);

    // 4. Inventory
    int32_t inv_count = game_player->inventory.items.size;
    fwrite(&inv_count, sizeof(int32_t), 1, f);

    for (int i = 0; i < inv_count; ++i) {
        Item* item = (Item*)arraylist_get(&game_player->inventory.items, i);
        uint8_t item_id = (uint8_t)item->id;
        fwrite(&item_id, sizeof(uint8_t), 1, f);

        if (item->id == RESOURCE) {
            int32_t res_idx = get_resource_index(item->add.resource.resource);
            int32_t count = item->add.resource.count;
            fwrite(&res_idx, sizeof(int32_t), 1, f);
            fwrite(&count, sizeof(int32_t), 1, f);
        } else if (item->id == TOOL) {
            int32_t t_type = (int32_t)item->add.tool.type;
            int32_t t_level = item->add.tool.level;
            fwrite(&t_type, sizeof(int32_t), 1, f);
            fwrite(&t_level, sizeof(int32_t), 1, f);
        } else if (item->id == FURNITURE) {
            int32_t f_type = item->add.furniture.furniture ? (int32_t)item->add.furniture.furniture->entity.type : WORKBENCH;
            fwrite(&f_type, sizeof(int32_t), 1, f);
        }
    }

    // 5. Levels (5 levels: 0 to 4)
    for (int lvl = 0; lvl < 5; ++lvl) {
        Level* l = &game_levels[lvl];
        int32_t w = l->w;
        int32_t h = l->h;
        int32_t depth = l->depth;
        int32_t dirtColor = l->dirtColor;
        int32_t grassColor = l->grassColor;
        int32_t sandColor = l->sandColor;
        int32_t monsterDensity = l->monsterDensity;

        fwrite(&w, sizeof(int32_t), 1, f);
        fwrite(&h, sizeof(int32_t), 1, f);
        fwrite(&depth, sizeof(int32_t), 1, f);
        fwrite(&dirtColor, sizeof(int32_t), 1, f);
        fwrite(&grassColor, sizeof(int32_t), 1, f);
        fwrite(&sandColor, sizeof(int32_t), 1, f);
        fwrite(&monsterDensity, sizeof(int32_t), 1, f);

        fwrite(l->tiles, sizeof(unsigned char), w * h, f);
        fwrite(l->data, sizeof(unsigned char), w * h, f);
    }

    fclose(f);
    printf("Game saved successfully to %s!\n", filepath);
    return 1;
}

int load_game(const char* filepath) {
    if (!filepath) filepath = get_save_path();

    FILE* f = fopen(filepath, "rb");
    if (!f) {
        f = fopen("save.dat", "rb");
        if (!f) return 0;
    }

    uint32_t magic = 0, version = 0;
    if (fread(&magic, sizeof(uint32_t), 1, f) != 1 ||
        fread(&version, sizeof(uint32_t), 1, f) != 1 ||
        magic != SAVE_MAGIC || version != SAVE_VERSION) {
        fclose(f);
        return 0;
    }

    // Free existing levels and player
    for (int i = 0; i < 5; ++i) {
        level_free(game_levels + i);
    }
    if (game_player) {
        call_entity_free(&game_player->mob.entity);
        free(game_player);
        game_player = NULL;
    }

    // Read game state
    int32_t gTime = 0, cLevel = 3, hWon = 0, wTimer = 0;
    fread(&gTime, sizeof(int32_t), 1, f);
    fread(&cLevel, sizeof(int32_t), 1, f);
    fread(&hWon, sizeof(int32_t), 1, f);
    fread(&wTimer, sizeof(int32_t), 1, f);

    game_gameTime = gTime;
    game_currentLevel = cLevel;
    game_hasWon = (char)hWon;
    game_wonTimer = wTimer;

    // Read player state
    int32_t px = 0, py = 0, phealth = 10, pmaxHealth = 10, pdir = 0;
    int32_t pstamina = 10, pmaxStamina = 10, pscore = 0;
    fread(&px, sizeof(int32_t), 1, f);
    fread(&py, sizeof(int32_t), 1, f);
    fread(&phealth, sizeof(int32_t), 1, f);
    fread(&pmaxHealth, sizeof(int32_t), 1, f);
    fread(&pdir, sizeof(int32_t), 1, f);
    fread(&pstamina, sizeof(int32_t), 1, f);
    fread(&pmaxStamina, sizeof(int32_t), 1, f);
    fread(&pscore, sizeof(int32_t), 1, f);

    // Create player
    game_player = (Player*)malloc(sizeof(Player));
    player_create(game_player);
    game_player->mob.entity.x = px;
    game_player->mob.entity.y = py;
    game_player->mob.health = phealth;
    game_player->mob.maxHealth = pmaxHealth;
    game_player->mob.dir = pdir;
    game_player->stamina = pstamina;
    game_player->maxStamina = pmaxStamina;
    game_player->score = pscore;

    // Read inventory
    int32_t inv_count = 0;
    fread(&inv_count, sizeof(int32_t), 1, f);

    for (int i = 0; i < inv_count; ++i) {
        uint8_t item_id = 0;
        fread(&item_id, sizeof(uint8_t), 1, f);

        Item* item = (Item*)malloc(sizeof(Item));
        memset(item, 0, sizeof(Item));

        if (item_id == RESOURCE) {
            int32_t res_idx = 0, count = 1;
            fread(&res_idx, sizeof(int32_t), 1, f);
            fread(&count, sizeof(int32_t), 1, f);
            if (res_idx >= 0 && (size_t)res_idx < NUM_RESOURCES) {
                resourceitem_create_cnt(item, resource_list[res_idx], count);
                inventory_addItem(&game_player->inventory, item);
            } else {
                free(item);
            }
        } else if (item_id == TOOL) {
            int32_t t_type = 0, t_level = 0;
            fread(&t_type, sizeof(int32_t), 1, f);
            fread(&t_level, sizeof(int32_t), 1, f);
            toolitem_create(item, (ToolType)t_type, t_level);
            inventory_addItem(&game_player->inventory, item);
        } else if (item_id == FURNITURE) {
            int32_t f_type = 0;
            fread(&f_type, sizeof(int32_t), 1, f);
            Furniture* furn = (Furniture*)malloc(sizeof(Furniture));
            if (f_type == WORKBENCH) workbench_create((Workbench*)furn);
            else if (f_type == FURNACE) furnace_create((Furnace*)furn);
            else if (f_type == OVEN) oven_create((Oven*)furn);
            else if (f_type == ANVIL) anvil_create((Anvil*)furn);
            else if (f_type == CHEST) chest_create((Chest*)furn);
            else if (f_type == LANTERN) lantern_create((Lantern*)furn);
            else workbench_create((Workbench*)furn);

            furnitureitem_create(item, furn);
            inventory_addItem(&game_player->inventory, item);
        } else if (item_id == POWERGLOVE) {
            powergloveitem_create(item);
            inventory_addItem(&game_player->inventory, item);
        } else {
            free(item);
        }
    }

    // Read 5 Levels
    for (int lvl = 0; lvl < 5; ++lvl) {
        Level* l = &game_levels[lvl];
        int32_t w = 128, h = 128, depth = 0;
        int32_t dirtColor = 322, grassColor = 141, sandColor = 550, monsterDensity = 8;

        fread(&w, sizeof(int32_t), 1, f);
        fread(&h, sizeof(int32_t), 1, f);
        fread(&depth, sizeof(int32_t), 1, f);
        fread(&dirtColor, sizeof(int32_t), 1, f);
        fread(&grassColor, sizeof(int32_t), 1, f);
        fread(&sandColor, sizeof(int32_t), 1, f);
        fread(&monsterDensity, sizeof(int32_t), 1, f);

        random_set_seed(&l->random, getTimeMS());
        create_arraylist(&l->entities);

        l->w = w;
        l->h = h;
        l->depth = depth;
        l->dirtColor = dirtColor;
        l->grassColor = grassColor;
        l->sandColor = sandColor;
        l->monsterDensity = monsterDensity;

        l->tiles = (unsigned char*)malloc(sizeof(unsigned char) * w * h);
        l->data = (unsigned char*)malloc(sizeof(unsigned char) * w * h);

        fread(l->tiles, sizeof(unsigned char), w * h, f);
        fread(l->data, sizeof(unsigned char), w * h, f);

        l->entitiesInTiles = (ArrayList*)malloc(sizeof(ArrayList) * w * h);
        for (int i = 0; i < w * h; ++i) {
            create_arraylist(l->entitiesInTiles + i);
        }

        level_trySpawn(l, 3000);
    }

    // Connect level current and add player
    if (game_currentLevel < 0 || game_currentLevel > 4) game_currentLevel = 3;
    game_level = &game_levels[game_currentLevel];
    level_addEntity(game_level, &game_player->mob.entity);

    isingame = 1;
    game_set_menu(0);

    fclose(f);
    printf("Game loaded successfully from %s!\n", filepath);
    return 1;
}
