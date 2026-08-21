#ifndef SAVE_H
#define SAVE_H

#include "game.h"

// Returns 1 on success, 0 on failure
int save_game(const char* filepath);

// Returns 1 on success, 0 on failure
int load_game(const char* filepath);

// Returns 1 if save file exists and is valid
int save_exists(const char* filepath);

// Default save path for Nintendo Wii SD Card / Local
const char* get_save_path(void);

#endif // SAVE_H
