#include "pause_menu.h"
#include "../inputhandler.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../game.h"
#include "../save.h"
#include "../sound.h"
#include "../lang.h"

#include <string.h>
#include <stdio.h>

static int pause_selected = 0;
static int save_notification_timer = 0;

const menu_vt pausemenu_vt = {
    &pausemenu_tick,
    &pausemenu_render,
    &pausemenu_init
};

void pausemenu_init(void) {
    pause_selected = 0;
    save_notification_timer = 0;
}

void pausemenu_render(Screen* screen) {
    // Draw a semi-transparent or bordered frame in the middle
    int w = 22;
    int h = 14;
    int xo = (screen->w / 8 - w) / 2;
    int yo = (screen->h / 8 - h) / 2;

    const char* pTitle = _T(STR_PAUSE);
    font_renderFrame(screen, (char*)pTitle, strlen(pTitle), xo, yo, xo + w, yo + h);

    const char* options[5];
    options[0] = _T(STR_RESUME);
    options[1] = _T(STR_SAVE_GAME);
    options[2] = _T(STR_LOAD_GAME);
    options[3] = _T(STR_LANGUAGE);
    options[4] = _T(STR_QUIT_TITLE);

    for (int i = 0; i < 5; ++i) {
        char buf[64] = {0};
        int col = getColor4(0, 222, 222, 222);

        if (i == pause_selected) {
            col = getColor4(0, 555, 555, 555);
            snprintf(buf, sizeof(buf), "> %s <", options[i]);
        } else {
            snprintf(buf, sizeof(buf), "%s", options[i]);
        }

        int optLen = strlen(buf);
        font_draw(buf, optLen, screen, (screen->w - optLen * 8) / 2, (yo + 2 + i * 2) * 8, col);
    }

    if (save_notification_timer > 0) {
        const char* msg = _T(STR_SAVED_MSG);
        int mLen = strlen(msg);
        font_draw((char*)msg, mLen, screen, (screen->w - mLen * 8) / 2, (yo + 12) * 8, getColor4(0, 151, 551, 555));
    }
}

void pausemenu_tick(void) {
    if (save_notification_timer > 0) {
        --save_notification_timer;
    }

    if (pause_key.clicked) {
        // Resume game when pressing pause again
        sound_play(SND_CONFIRM);
        game_set_menu(0);
        return;
    }

    if (up.clicked) {
        --pause_selected;
        sound_play(SND_SELECT);
    }
    if (down.clicked) {
        ++pause_selected;
        sound_play(SND_SELECT);
    }

    if (pause_selected < 0) pause_selected = 0;
    if (pause_selected > 4) pause_selected = 4;

    if (attack.clicked) {
        sound_play(SND_CONFIRM);

        if (pause_selected == 0) {
            // Resume
            game_set_menu(0);
        } else if (pause_selected == 1) {
            // Save Game
            save_game(NULL);
            save_notification_timer = 90; // Show notification for 1.5 seconds
        } else if (pause_selected == 2) {
            // Load Game
            load_game(NULL);
        } else if (pause_selected == 3) {
            // Change Language
            lang_next();
        } else if (pause_selected == 4) {
            // Save & Quit to Title
            save_game(NULL);
            isingame = 0;
            game_set_menu(mid_TITLE);
        }
    }
}
