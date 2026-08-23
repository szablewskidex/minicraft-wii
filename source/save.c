#include "save.h"
#include "gamemode.h"
#include "lang.h"
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
#include "entity/bed.h"
#include "entity/door.h"
#include "entity/cow.h"
#include "entity/chicken.h"
#include "entity/pig.h"
#include "entity/sheep.h"
#include "entity/crab.h"
#include "entity/frog.h"
#include "entity/_entity_caller.h"

#define SAVE_MAGIC_V1 0x4D435749 // "MCWI"
#define SAVE_MAGIC_V2 0x4D435732 // "MCW2"
#define SAVE_MAGIC_V3 0x4D435733 // "MCW3"
#define SAVE_VERSION_1 1
#define SAVE_VERSION_2 2
#define SAVE_VERSION_3 3

static Resource* resource_list[] = {
    &wood, &stone, &flower, &acorn, &dirt, &sand, &cactusFlower, &seeds,
    &wheat, &bread, &apple, &coal, &ironOre, &goldOre, &ironIngot, &goldIngot,
    &slime, &glass, &cloth, &cloud, &gem, &leather, &boat,
    &leatherArmor, &ironArmor, &goldArmor, &gemArmor,
    &bow, &arrow, &fishingRod, &rawFish, &cookedFish, &shears, &carrot, &potato,
    &rawBeef, &cookedSteak, &rawPork, &cookedPork, &woodPlank, &woodWallItem
};
#define NUM_RESOURCES (sizeof(resource_list) / sizeof(Resource*))

static int get_resource_index(Resource* res) {
    if (!res) return -1;
    for (size_t i = 0; i < NUM_RESOURCES; ++i) {
        if (resource_list[i] == res) return (int)i;
    }
    return -1;
}

static void get_slot_path(int slot, char* buf, size_t buflen) {
    if (slot < 1 || slot > 3) slot = 1;
#ifdef __wii__
    snprintf(buf, buflen, "sd:/apps/minicraft/save_slot%d.dat", slot);
#else
    snprintf(buf, buflen, "save_slot%d.dat", slot);
#endif
}

static void save_inventory(Inventory* inv, FILE* f) {
    int32_t inv_count = inv->items.size;
    fwrite(&inv_count, sizeof(int32_t), 1, f);
    for (int i = 0; i < inv_count; ++i) {
        Item* item = (Item*)arraylist_get(&inv->items, i);
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
}

static void load_inventory(Inventory* inv, FILE* f) {
    inventory_free(inv);
    inventory_create(inv);

    int32_t inv_count = 0;
    if (fread(&inv_count, sizeof(int32_t), 1, f) != 1) return;
    for (int i = 0; i < inv_count; ++i) {
        uint8_t item_id = 0;
        if (fread(&item_id, sizeof(uint8_t), 1, f) != 1) break;
        Item* item = (Item*)malloc(sizeof(Item));
        memset(item, 0, sizeof(Item));
        if (item_id == RESOURCE) {
            int32_t res_idx = 0, count = 1;
            fread(&res_idx, sizeof(int32_t), 1, f);
            fread(&count, sizeof(int32_t), 1, f);
            if (res_idx >= 0 && (size_t)res_idx < NUM_RESOURCES) {
                resourceitem_create_cnt(item, resource_list[res_idx], count);
                inventory_addItem(inv, item);
            }
            free(item);
        } else if (item_id == TOOL) {
            int32_t t_type = 0, t_level = 0;
            fread(&t_type, sizeof(int32_t), 1, f);
            fread(&t_level, sizeof(int32_t), 1, f);
            toolitem_create(item, (ToolType)t_type, t_level);
            inventory_addItem(inv, item);
            free(item);
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
            else if (f_type == BED) bed_create((Bed*)furn);
            else if (f_type == DOOR) door_create((Door*)furn);
            else workbench_create((Workbench*)furn);
            furnitureitem_create(item, furn);
            inventory_addItem(inv, item);
            free(item);
        } else if (item_id == POWERGLOVE) {
            powergloveitem_create(item);
            inventory_addItem(inv, item);
            free(item);
        } else {
            free(item);
        }
    }
}

static FILE* open_slot_file_read(int slot) {
    char path[128];
    get_slot_path(slot, path, sizeof(path));
    FILE* f = fopen(path, "rb");
    if (f) return f;

    char fallback[64];
    snprintf(fallback, sizeof(fallback), "save_slot%d.dat", slot);
    f = fopen(fallback, "rb");
    if (f) return f;

    if (slot == 1) {
#ifdef __wii__
        f = fopen("sd:/apps/minicraft/save.dat", "rb");
        if (f) return f;
#endif
        f = fopen("save.dat", "rb");
        if (f) return f;
    }
    return NULL;
}

int slot_exists(int slot) {
    FILE* f = open_slot_file_read(slot);
    if (!f) return 0;

    uint32_t magic = 0, version = 0;
    if (fread(&magic, sizeof(uint32_t), 1, f) != 1 ||
        fread(&version, sizeof(uint32_t), 1, f) != 1) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return ((magic == SAVE_MAGIC_V3 && version == SAVE_VERSION_3) ||
            (magic == SAVE_MAGIC_V2 && version == SAVE_VERSION_2) ||
            (magic == SAVE_MAGIC_V1 && version == SAVE_VERSION_1));
}

int get_slot_info(int slot, SlotInfo* info) {
    memset(info, 0, sizeof(SlotInfo));
    FILE* f = open_slot_file_read(slot);
    if (!f) return 0;

    uint32_t magic = 0, version = 0;
    if (fread(&magic, sizeof(uint32_t), 1, f) != 1 ||
        fread(&version, sizeof(uint32_t), 1, f) != 1) {
        fclose(f);
        return 0;
    }

    if ((version == SAVE_VERSION_3 && magic == SAVE_MAGIC_V3) ||
        (version == SAVE_VERSION_2 && magic == SAVE_MAGIC_V2)) {
        int32_t mode = 0, wsize = 128, gTime = 0, score = 0;
        fread(&mode, sizeof(int32_t), 1, f);
        fread(&wsize, sizeof(int32_t), 1, f);
        fread(&gTime, sizeof(int32_t), 1, f);
        int32_t dummy[4];
        fread(dummy, sizeof(int32_t), 4, f);
        int32_t dummy2[7];
        fread(dummy2, sizeof(int32_t), 7, f);
        fread(&score, sizeof(int32_t), 1, f);
        fclose(f);

        info->exists = 1;
        info->mode = (GameMode)mode;
        info->size = (WorldSize)wsize;
        info->gameTime = gTime;
        info->score = score;
        return 1;
    } else if (version == SAVE_VERSION_1 && magic == SAVE_MAGIC_V1) {
        int32_t gTime = 0, score = 0;
        fread(&gTime, sizeof(int32_t), 1, f);
        int32_t dummy[4];
        fread(dummy, sizeof(int32_t), 4, f);
        int32_t dummy2[7];
        fread(dummy2, sizeof(int32_t), 7, f);
        fread(&score, sizeof(int32_t), 1, f);
        fclose(f);

        info->exists = 1;
        info->mode = MODE_SURVIVAL;
        info->size = WORLD_NORMAL;
        info->gameTime = gTime;
        info->score = score;
        return 1;
    }

    fclose(f);
    return 0;
}

int delete_slot(int slot) {
    char path[128];
    get_slot_path(slot, path, sizeof(path));
    remove(path);
    char fallback[64];
    snprintf(fallback, sizeof(fallback), "save_slot%d.dat", slot);
    remove(fallback);
    if (slot == 1) {
#ifdef __wii__
        remove("sd:/apps/minicraft/save.dat");
#endif
        remove("save.dat");
    }
    return 1;
}

int save_slot(int slot) {
    if (!game_player || !isingame) return 0;
    if (slot < 1 || slot > 3) slot = 1;

    char path[128];
    get_slot_path(slot, path, sizeof(path));
    FILE* f = fopen(path, "wb");
    if (!f) {
        char fallback[64];
        snprintf(fallback, sizeof(fallback), "save_slot%d.dat", slot);
        f = fopen(fallback, "wb");
        if (!f) return 0;
    }

    // 1. Header (Format V3)
    uint32_t magic = SAVE_MAGIC_V3;
    uint32_t version = SAVE_VERSION_3;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    fwrite(&version, sizeof(uint32_t), 1, f);

    // 2. Game Mode & World Info
    int32_t sMode = (int32_t)g_gameMode;
    int32_t sWSize = (int32_t)g_worldSize;
    int32_t gTime = game_gameTime;
    int32_t cLevel = game_currentLevel;
    int32_t hWon = game_hasWon;
    int32_t wTimer = game_wonTimer;
    int32_t sLang = (int32_t)g_currentLanguage;

    fwrite(&sMode, sizeof(int32_t), 1, f);
    fwrite(&sWSize, sizeof(int32_t), 1, f);
    fwrite(&gTime, sizeof(int32_t), 1, f);
    fwrite(&cLevel, sizeof(int32_t), 1, f);
    fwrite(&hWon, sizeof(int32_t), 1, f);
    fwrite(&wTimer, sizeof(int32_t), 1, f);
    fwrite(&sLang, sizeof(int32_t), 1, f);

    // 3. Player state + EXP + Level + Armor
    int32_t px = game_player->mob.entity.x;
    int32_t py = game_player->mob.entity.y;
    int32_t phealth = game_player->mob.health;
    int32_t pmaxHealth = game_player->mob.maxHealth;
    int32_t pdir = game_player->mob.dir;
    int32_t pstamina = game_player->stamina;
    int32_t pmaxStamina = game_player->maxStamina;
    int32_t pscore = game_player->score;
    int32_t pexp = game_player->exp;
    int32_t plevel = game_player->level;
    int32_t pmaxExp = game_player->maxExp;
    int32_t parmorIdx = get_resource_index(game_player->armor);
    int32_t parmorDef = game_player->armorDefense;

    fwrite(&px, sizeof(int32_t), 1, f);
    fwrite(&py, sizeof(int32_t), 1, f);
    fwrite(&phealth, sizeof(int32_t), 1, f);
    fwrite(&pmaxHealth, sizeof(int32_t), 1, f);
    fwrite(&pdir, sizeof(int32_t), 1, f);
    fwrite(&pstamina, sizeof(int32_t), 1, f);
    fwrite(&pmaxStamina, sizeof(int32_t), 1, f);
    fwrite(&pscore, sizeof(int32_t), 1, f);
    fwrite(&pexp, sizeof(int32_t), 1, f);
    fwrite(&plevel, sizeof(int32_t), 1, f);
    fwrite(&pmaxExp, sizeof(int32_t), 1, f);
    fwrite(&parmorIdx, sizeof(int32_t), 1, f);
    fwrite(&parmorDef, sizeof(int32_t), 1, f);

    // 4. Inventory (Safely includes activeItem in hand!)
    Item* heldItem = game_player->activeItem;
    if (heldItem) {
        arraylist_pushTo(&game_player->inventory.items, 0, heldItem);
    }
    save_inventory(&game_player->inventory, f);
    if (heldItem) {
        arraylist_removeId(&game_player->inventory.items, 0);
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

        // Save furniture, chests, AND farm animals!
        int32_t num_entities = 0;
        for (int i = 0; i < l->entities.size; ++i) {
            Entity* e = l->entities.elements[i];
            if (entity_isfurniture(e) || entity_isAnimal(e)) num_entities++;
        }
        fwrite(&num_entities, sizeof(int32_t), 1, f);

        for (int i = 0; i < l->entities.size; ++i) {
            Entity* e = l->entities.elements[i];
            if (entity_isfurniture(e)) {
                int32_t kind = 1; // 1 = Furniture
                int32_t type = (int32_t)e->type;
                int32_t ex = e->x, ey = e->y;
                int32_t health = 0;
                fwrite(&kind, sizeof(int32_t), 1, f);
                fwrite(&type, sizeof(int32_t), 1, f);
                fwrite(&ex, sizeof(int32_t), 1, f);
                fwrite(&ey, sizeof(int32_t), 1, f);
                fwrite(&health, sizeof(int32_t), 1, f);

                if (e->type == CHEST) {
                    Chest* chest = (Chest*)e;
                    save_inventory(&chest->inventory, f);
                }
            } else if (entity_isAnimal(e)) {
                int32_t kind = 2; // 2 = Animal
                int32_t type = (int32_t)e->type;
                int32_t ex = e->x, ey = e->y;
                int32_t health = ((Mob*)e)->health;
                fwrite(&kind, sizeof(int32_t), 1, f);
                fwrite(&type, sizeof(int32_t), 1, f);
                fwrite(&ex, sizeof(int32_t), 1, f);
                fwrite(&ey, sizeof(int32_t), 1, f);
                fwrite(&health, sizeof(int32_t), 1, f);
            }
        }
    }

    fclose(f);
    printf("Game saved to Slot %d (V3)!\n", slot);
    return 1;
}

int load_slot(int slot) {
    if (slot < 1 || slot > 3) slot = 1;

    FILE* f = open_slot_file_read(slot);
    if (!f) return 0;

    uint32_t magic = 0, version = 0;
    if (fread(&magic, sizeof(uint32_t), 1, f) != 1 ||
        fread(&version, sizeof(uint32_t), 1, f) != 1) {
        fclose(f);
        return 0;
    }

    if (!((magic == SAVE_MAGIC_V3 && version == SAVE_VERSION_3) ||
          (magic == SAVE_MAGIC_V2 && version == SAVE_VERSION_2) ||
          (magic == SAVE_MAGIC_V1 && version == SAVE_VERSION_1))) {
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

    int32_t sMode = 0, sWSize = 128, gTime = 0, cLevel = 3, hWon = 0, wTimer = 0, sLang = 0;

    if (version == SAVE_VERSION_3 || version == SAVE_VERSION_2) {
        fread(&sMode, sizeof(int32_t), 1, f);
        fread(&sWSize, sizeof(int32_t), 1, f);
        fread(&gTime, sizeof(int32_t), 1, f);
        fread(&cLevel, sizeof(int32_t), 1, f);
        fread(&hWon, sizeof(int32_t), 1, f);
        fread(&wTimer, sizeof(int32_t), 1, f);
        if (fread(&sLang, sizeof(int32_t), 1, f) == 1) {
            if (sLang >= 0 && sLang < LANG_COUNT) {
                g_currentLanguage = (Language)sLang;
            }
        }
    } else {
        // Version 1 format
        sMode = (int32_t)MODE_SURVIVAL;
        sWSize = 128;
        fread(&gTime, sizeof(int32_t), 1, f);
        fread(&cLevel, sizeof(int32_t), 1, f);
        fread(&hWon, sizeof(int32_t), 1, f);
        fread(&wTimer, sizeof(int32_t), 1, f);
        if (fread(&sLang, sizeof(int32_t), 1, f) == 1) {
            if (sLang >= 0 && sLang < LANG_COUNT) {
                g_currentLanguage = (Language)sLang;
            }
        }
    }

    g_gameMode = (GameMode)sMode;
    g_worldSize = (WorldSize)sWSize;
    g_currentSlot = slot;
    game_gameTime = gTime;
    game_currentLevel = cLevel;
    game_hasWon = (char)hWon;
    game_wonTimer = wTimer;

    // Read player state
    int32_t px = 0, py = 0, phealth = 10, pmaxHealth = 10, pdir = 0;
    int32_t pstamina = 10, pmaxStamina = 10, pscore = 0;
    int32_t pexp = 0, plevel = 1, pmaxExp = 50, parmorIdx = -1, parmorDef = 0;

    fread(&px, sizeof(int32_t), 1, f);
    fread(&py, sizeof(int32_t), 1, f);
    fread(&phealth, sizeof(int32_t), 1, f);
    fread(&pmaxHealth, sizeof(int32_t), 1, f);
    fread(&pdir, sizeof(int32_t), 1, f);
    fread(&pstamina, sizeof(int32_t), 1, f);
    fread(&pmaxStamina, sizeof(int32_t), 1, f);
    fread(&pscore, sizeof(int32_t), 1, f);

    if (version == SAVE_VERSION_3) {
        fread(&pexp, sizeof(int32_t), 1, f);
        fread(&plevel, sizeof(int32_t), 1, f);
        fread(&pmaxExp, sizeof(int32_t), 1, f);
        fread(&parmorIdx, sizeof(int32_t), 1, f);
        fread(&parmorDef, sizeof(int32_t), 1, f);
    }

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
    game_player->exp = pexp;
    game_player->level = plevel > 0 ? plevel : 1;
    game_player->maxExp = pmaxExp > 0 ? pmaxExp : (game_player->level * 50 + 50);

    if (parmorIdx >= 0 && (size_t)parmorIdx < NUM_RESOURCES) {
        game_player->armor = resource_list[parmorIdx];
        game_player->armorDefense = parmorDef;
    } else {
        game_player->armor = NULL;
        game_player->armorDefense = 0;
    }

    // Read inventory
    load_inventory(&game_player->inventory, f);

    // Set first inventory item as activeItem if available
    if (game_player->inventory.items.size > 0) {
        game_player->activeItem = (Item*)arraylist_removeId(&game_player->inventory.items, 0);
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

        if (version == SAVE_VERSION_3) {
            int32_t num_entities = 0;
            if (fread(&num_entities, sizeof(int32_t), 1, f) == 1) {
                for (int i = 0; i < num_entities; ++i) {
                    int32_t kind = 0, type = 0, ex = 0, ey = 0, health = 0;
                    fread(&kind, sizeof(int32_t), 1, f);
                    fread(&type, sizeof(int32_t), 1, f);
                    fread(&ex, sizeof(int32_t), 1, f);
                    fread(&ey, sizeof(int32_t), 1, f);
                    fread(&health, sizeof(int32_t), 1, f);

                    if (kind == 1) {
                        // Furniture
                        Furniture* furn = entity_createFurniture((EntityId)type);
                        if (furn) {
                            furn->entity.x = ex;
                            furn->entity.y = ey;
                            if (type == CHEST) {
                                Chest* chest = (Chest*)furn;
                                load_inventory(&chest->inventory, f);
                            }
                            level_addEntity(l, &furn->entity);
                        }
                    } else if (kind == 2) {
                        // Farm Animal
                        Mob* animal = entity_createAnimal((EntityId)type);
                        if (animal) {
                            animal->entity.x = ex;
                            animal->entity.y = ey;
                            if (health > 0) animal->health = health;
                            level_addEntity(l, &animal->entity);
                        }
                    }
                }
            }
        } else if (version == SAVE_VERSION_2) {
            int32_t num_entities = 0;
            if (fread(&num_entities, sizeof(int32_t), 1, f) == 1) {
                for (int i = 0; i < num_entities; ++i) {
                    int32_t type = 0, ex = 0, ey = 0;
                    fread(&type, sizeof(int32_t), 1, f);
                    fread(&ex, sizeof(int32_t), 1, f);
                    fread(&ey, sizeof(int32_t), 1, f);

                    Furniture* furn = entity_createFurniture((EntityId)type);
                    if (furn) {
                        furn->entity.x = ex;
                        furn->entity.y = ey;
                        if (type == CHEST) {
                            Chest* chest = (Chest*)furn;
                            load_inventory(&chest->inventory, f);
                        }
                        level_addEntity(l, &furn->entity);
                    }
                }
            }
        }

        if (lvl != 3) {
            level_trySpawn(l, 2000);
        }
    }

    if (game_currentLevel < 0 || game_currentLevel > 4) game_currentLevel = 3;
    game_level = &game_levels[game_currentLevel];
    level_addEntity(game_level, &game_player->mob.entity);

    isingame = 1;
    game_set_menu(0);

    fclose(f);
    printf("Game loaded from Slot %d (V3)!\n", slot);
    return 1;
}

int save_game(const char* filepath) {
    (void)filepath;
    return save_slot(g_currentSlot);
}

int load_game(const char* filepath) {
    (void)filepath;
    return load_slot(g_currentSlot);
}

int save_exists(const char* filepath) {
    (void)filepath;
    return slot_exists(1) || slot_exists(2) || slot_exists(3);
}
