#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include "gamemode.h"

typedef struct {
    int exists;
    GameMode mode;
    WorldSize size;
    int gameTime;
    int score;
} SlotInfo;

int save_slot(int slot);
int load_slot(int slot);
int slot_exists(int slot);
int get_slot_info(int slot, SlotInfo* info);
int delete_slot(int slot);

// Helpers for current slot
int save_game(const char* filepath);
int load_game(const char* filepath);
int save_exists(const char* filepath);

#endif // SAVE_H
