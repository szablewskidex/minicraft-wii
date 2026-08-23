#include "slot_menu.h"
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

int slotmenu_mode = 0; // 0 = Load, 1 = New Game
static int slot_selected = 0;
static SlotInfo slots[3];

const menu_vt slotmenu_vt = {
    &slotmenu_tick,
    &slotmenu_render,
    &slotmenu_init
};

void slotmenu_init(void) {
    slot_selected = 0;
    for (int i = 0; i < 3; ++i) {
        get_slot_info(i + 1, &slots[i]);
    }
}

void slotmenu_render(Screen* screen) {
    clear_screen(screen, 0);

    const char* title = (slotmenu_mode == 0) ? 
        ((g_currentLanguage == LANG_PL) ? "Wybierz Zapis" : "Select Save Slot") :
        ((g_currentLanguage == LANG_PL) ? "Wybierz Slot Nowej Gry" : "Select Slot for New Game");

    int tLen = strlen(title);
    int tY = (screen->h < 150) ? 8 : 16;
    font_draw((char*)title, tLen, screen, (screen->w - tLen * 8) / 2, tY, getColor4(0, 555, 555, 555));

    for (int i = 0; i < 3; ++i) {
        int yo = (screen->h < 150) ? (24 + i * 26) : (40 + i * 40);
        char line1[64] = {0};
        char line2[64] = {0};

        if (slots[i].exists) {
            int mins = (slots[i].gameTime / 60) / 60;
            snprintf(line1, sizeof(line1), "Slot %d: %s (%dx%d)", 
                i + 1, gamemode_get_name(slots[i].mode), (int)slots[i].size, (int)slots[i].size);
            snprintf(line2, sizeof(line2), "  Time: %dm | Score: %d", mins, slots[i].score);
        } else {
            snprintf(line1, sizeof(line1), "Slot %d: %s", i + 1, (g_currentLanguage == LANG_PL) ? "[Pusty]" : "[Empty]");
            snprintf(line2, sizeof(line2), "  ---");
        }

        int col1 = (i == slot_selected) ? getColor4(0, 555, 555, 555) : getColor4(0, 333, 333, 333);
        int col2 = (i == slot_selected) ? getColor4(0, 550, 550, 550) : getColor4(0, 222, 222, 222);

        char prefix[64];
        if (i == slot_selected) {
            snprintf(prefix, sizeof(prefix), "> %s", line1);
        } else {
            snprintf(prefix, sizeof(prefix), "  %s", line1);
        }

        int l1Len = strlen(prefix);
        int l2Len = strlen(line2);
        font_draw(prefix, l1Len, screen, (screen->w - l1Len * 8) / 2, yo, col1);
        font_draw(line2, l2Len, screen, (screen->w - l2Len * 8) / 2, yo + 10, col2);
    }

    char back[64] = "Back to Title";
    if (g_currentLanguage == LANG_PL) strcpy(back, "Powrot do Menu");
    int bCol = (slot_selected == 3) ? getColor4(0, 555, 555, 555) : getColor4(0, 222, 222, 222);
    char bBuf[64];
    if (slot_selected == 3) snprintf(bBuf, sizeof(bBuf), "> %s <", back);
    else snprintf(bBuf, sizeof(bBuf), "  %s", back);
    int bLen = strlen(bBuf);
    int backY = (screen->h < 150) ? 110 : 175;
    font_draw(bBuf, bLen, screen, (screen->w - bLen * 8) / 2, backY, bCol);
}

void slotmenu_tick(void) {
    if (up.clicked) {
        --slot_selected;
        sound_play(SND_SELECT);
    }
    if (down.clicked) {
        ++slot_selected;
        sound_play(SND_SELECT);
    }

    if (slot_selected < 0) slot_selected = 0;
    if (slot_selected > 3) slot_selected = 3;

    if (attack.clicked || menu.clicked) {
        sound_play(SND_CONFIRM);

        if (slot_selected == 3) {
            // Back to Title
            game_set_menu(mid_TITLE);
            return;
        }

        int chosen_slot = slot_selected + 1;
        g_currentSlot = chosen_slot;

        if (slotmenu_mode == 0) {
            // Load Game from chosen slot
            if (slots[slot_selected].exists) {
                load_slot(chosen_slot);
            }
        } else {
            // New Game in chosen slot -> Go to New Game options (mode & world size)
            game_set_menu(mid_NEW_GAME);
        }
    }
}
