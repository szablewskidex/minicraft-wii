#include "newgame_menu.h"
#include "../inputhandler.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../game.h"
#include "../save.h"
#include "../sound.h"
#include "../lang.h"
#include "../gamemode.h"

#include <string.h>
#include <stdio.h>

static int newgame_selected = 0;

const menu_vt newgamemenu_vt = {
    &newgamemenu_tick,
    &newgamemenu_render,
    &newgamemenu_init
};

void newgamemenu_init(void) {
    newgame_selected = 0;
}

void newgamemenu_render(Screen* screen) {
    clear_screen(screen, 0);

    const char* title = (g_currentLanguage == LANG_PL) ? "Ustawienia Nowego Swiata" : "New World Settings";
    int tLen = strlen(title);
    font_draw((char*)title, tLen, screen, (screen->w - tLen * 8) / 2, 20, getColor4(0, 555, 555, 555));

    char opt0[64], opt1[64], opt2[64], opt3[64];
    snprintf(opt0, sizeof(opt0), "%s: %s", (g_currentLanguage == LANG_PL) ? "Tryb Gry" : "Gamemode", gamemode_get_name(g_gameMode));
    snprintf(opt1, sizeof(opt1), "%s: %s", (g_currentLanguage == LANG_PL) ? "Rozmiar" : "World Size", worldsize_get_name(g_worldSize));
    snprintf(opt2, sizeof(opt2), "[ %s ]", (g_currentLanguage == LANG_PL) ? "STWORZ SWIAT" : "CREATE WORLD");
    snprintf(opt3, sizeof(opt3), "%s", (g_currentLanguage == LANG_PL) ? "Wstecz" : "Back");

    const char* opts[4] = { opt0, opt1, opt2, opt3 };

    for (int i = 0; i < 4; ++i) {
        char buf[64] = {0};
        int col = getColor4(0, 222, 222, 222);

        if (i == newgame_selected) {
            col = getColor4(0, 555, 555, 555);
            snprintf(buf, sizeof(buf), "> %s <", opts[i]);
        } else {
            snprintf(buf, sizeof(buf), "%s", opts[i]);
        }

        int len = strlen(buf);
        font_draw(buf, len, screen, (screen->w - len * 8) / 2, 60 + i * 25, col);
    }
}

void newgamemenu_tick(void) {
    if (up.clicked) {
        --newgame_selected;
        sound_play(SND_SELECT);
    }
    if (down.clicked) {
        ++newgame_selected;
        sound_play(SND_SELECT);
    }

    if (newgame_selected < 0) newgame_selected = 0;
    if (newgame_selected > 3) newgame_selected = 3;

    if (left.clicked) {
        if (newgame_selected == 0) {
            g_gameMode = (GameMode)((g_gameMode + MODE_COUNT - 1) % MODE_COUNT);
            sound_play(SND_SELECT);
        } else if (newgame_selected == 1) {
            if (g_worldSize == WORLD_HUGE) g_worldSize = WORLD_NORMAL;
            else if (g_worldSize == WORLD_NORMAL) g_worldSize = WORLD_SMALL;
            else g_worldSize = WORLD_HUGE;
            sound_play(SND_SELECT);
        }
    }

    if (right.clicked) {
        if (newgame_selected == 0) {
            g_gameMode = (GameMode)((g_gameMode + 1) % MODE_COUNT);
            sound_play(SND_SELECT);
        } else if (newgame_selected == 1) {
            if (g_worldSize == WORLD_SMALL) g_worldSize = WORLD_NORMAL;
            else if (g_worldSize == WORLD_NORMAL) g_worldSize = WORLD_HUGE;
            else g_worldSize = WORLD_SMALL;
            sound_play(SND_SELECT);
        }
    }

    if (attack.clicked) {
        sound_play(SND_CONFIRM);

        if (newgame_selected == 0) {
            // Cycle Gamemode
            g_gameMode = (GameMode)((g_gameMode + 1) % MODE_COUNT);
        } else if (newgame_selected == 1) {
            // Cycle World Size
            if (g_worldSize == WORLD_SMALL) g_worldSize = WORLD_NORMAL;
            else if (g_worldSize == WORLD_NORMAL) g_worldSize = WORLD_HUGE;
            else g_worldSize = WORLD_SMALL;
        } else if (newgame_selected == 2) {
            // Start World!
            isingame = 1;
            game_reset();
            save_slot(g_currentSlot);
            game_set_menu(0);
        } else if (newgame_selected == 3) {
            // Back
            game_set_menu(mid_SLOT_SELECT);
        }
    }
}
