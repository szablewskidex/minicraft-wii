#ifndef LANG_H
#define LANG_H

typedef enum {
    LANG_PL = 0,
    LANG_EN = 1,
    LANG_DE = 2,
    LANG_COUNT = 3
} Language;

typedef enum {
    STR_START_GAME,
    STR_NEW_GAME,
    STR_CONTINUE,
    STR_HOW_TO_PLAY,
    STR_ABOUT,
    STR_LANGUAGE,
    STR_SCREEN_MODE,
    STR_HELP_MSG,
    STR_INVENTORY,
    STR_CRAFTING,
    STR_YOU_DIED,
    STR_YOU_WON,
    STR_TIME,
    STR_SCORE,
    STR_PRESS_TO_RESTART,
    STR_PAUSE,
    STR_RESUME,
    STR_SAVE_GAME,
    STR_LOAD_GAME,
    STR_QUIT_TITLE,
    STR_QUIT_NO_SAVE,
    STR_SAVED_MSG,
    STR_EXIT,
    STR_OPTIONS,
    STR_BACK,
    STR_SFX_VOLUME,
    STR_UI_VOLUME,
    STR_ASPECT_RATIO,
    STR_ASPECT_4_3,
    STR_ASPECT_16_9,
    STR_HAVE,
    STR_COST,
    STR_BUTTON_PROMPTS,
    STR_ON,
    STR_OFF,
    STR_COUNT
} StringID;

extern Language g_currentLanguage;

void lang_init(void);
void lang_next(void);
const char* lang_get(StringID id);
const char* lang_translate_item(const char* name);

#define _T(id) lang_get(id)

#endif // LANG_H
