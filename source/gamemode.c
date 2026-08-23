#include "gamemode.h"
#include "lang.h"

GameMode g_gameMode = MODE_SURVIVAL;
WorldSize g_worldSize = WORLD_NORMAL;
int g_currentSlot = 1;
int g_aspectRatio = 1; // 0 = 4:3, 1 = 16:9 Widescreen
int g_buttonPrompts = 1; // 0 = OFF, 1 = ON
int g_activeControllerType = 0; // 0 = GameCube, 1 = Wiimote

const char* gamemode_get_name(GameMode mode) {
    if (g_currentLanguage == LANG_PL) {
        switch (mode) {
            case MODE_SURVIVAL: return "Przetrwanie";
            case MODE_CREATIVE: return "Kreatywny";
            case MODE_HARDCORE: return "Hardcore";
            case MODE_SCORE:    return "Na Punkty";
            default: return "Przetrwanie";
        }
    } else if (g_currentLanguage == LANG_DE) {
        switch (mode) {
            case MODE_SURVIVAL: return "Überleben";
            case MODE_CREATIVE: return "Kreativ";
            case MODE_HARDCORE: return "Hardcore";
            case MODE_SCORE:    return "Punkte-Modus";
            default: return "Überleben";
        }
    } else {
        switch (mode) {
            case MODE_SURVIVAL: return "Survival";
            case MODE_CREATIVE: return "Creative";
            case MODE_HARDCORE: return "Hardcore";
            case MODE_SCORE:    return "Score Mode";
            default: return "Survival";
        }
    }
}

const char* worldsize_get_name(WorldSize size) {
    if (g_currentLanguage == LANG_PL) {
        switch (size) {
            case WORLD_SMALL:  return "Maly (64x64)";
            case WORLD_NORMAL: return "Normalny (128x128)";
            case WORLD_HUGE:   return "Wielki (256x256)";
            default: return "128x128";
        }
    } else if (g_currentLanguage == LANG_DE) {
        switch (size) {
            case WORLD_SMALL:  return "Klein (64x64)";
            case WORLD_NORMAL: return "Normal (128x128)";
            case WORLD_HUGE:   return "Riesig (256x256)";
            default: return "128x128";
        }
    } else {
        switch (size) {
            case WORLD_SMALL:  return "Small (64x64)";
            case WORLD_NORMAL: return "Normal (128x128)";
            case WORLD_HUGE:   return "Huge (256x256)";
            default: return "128x128";
        }
    }
}
