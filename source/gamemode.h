#ifndef GAMEMODE_H
#define GAMEMODE_H

typedef enum {
    MODE_SURVIVAL = 0,
    MODE_CREATIVE = 1,
    MODE_HARDCORE = 2,
    MODE_SCORE = 3,
    MODE_COUNT = 4
} GameMode;

typedef enum {
    WORLD_SMALL = 64,
    WORLD_NORMAL = 128,
    WORLD_HUGE = 256
} WorldSize;

extern GameMode g_gameMode;
extern WorldSize g_worldSize;
extern int g_currentSlot;

const char* gamemode_get_name(GameMode mode);
const char* worldsize_get_name(WorldSize size);

#endif // GAMEMODE_H
