#include "options_menu.h"
#include "../inputhandler.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../game.h"
#include "../sound.h"
#include "../lang.h"
#include "../gamemode.h"

#include <string.h>
#include <stdio.h>

static int options_selected = 0;

const menu_vt optionsmenu_vt = {
    &optionsmenu_tick,
    &optionsmenu_render,
    &optionsmenu_init
};

void optionsmenu_init(void) {
    options_selected = 0;
}

void optionsmenu_render(Screen* screen) {
    int w = 24;
    int h = 16;
    int xo = (screen->w / 8 - w) / 2;
    int yo = (screen->h / 8 - h) / 2;

    const char* pTitle = _T(STR_OPTIONS);
    font_renderFrame(screen, (char*)pTitle, strlen(pTitle), xo, yo, xo + w, yo + h);

    char opt0[64];
    const char* aspectName = (g_aspectRatio == 1) ? _T(STR_ASPECT_16_9) : _T(STR_ASPECT_4_3);
    snprintf(opt0, sizeof(opt0), "%s: %s", _T(STR_ASPECT_RATIO), aspectName);

    char opt1[64];
    snprintf(opt1, sizeof(opt1), "%s", _T(STR_LANGUAGE));

    char opt2[64];
    const char* promptState = (g_buttonPrompts == 1) ? _T(STR_ON) : _T(STR_OFF);
    snprintf(opt2, sizeof(opt2), "%s: %s", _T(STR_BUTTON_PROMPTS), promptState);

    char opt3[64];
    if (g_sfxVolume <= 0) {
        snprintf(opt3, sizeof(opt3), "%s: [OFF]", _T(STR_SFX_VOLUME));
    } else {
        snprintf(opt3, sizeof(opt3), "%s: %d0%%", _T(STR_SFX_VOLUME), g_sfxVolume);
    }

    char opt4[64];
    if (g_uiVolume <= 0) {
        snprintf(opt4, sizeof(opt4), "%s: [OFF]", _T(STR_UI_VOLUME));
    } else {
        snprintf(opt4, sizeof(opt4), "%s: %d0%%", _T(STR_UI_VOLUME), g_uiVolume);
    }

    char opt5[64];
    snprintf(opt5, sizeof(opt5), "%s", _T(STR_BACK));

    const char* lines[6] = { opt0, opt1, opt2, opt3, opt4, opt5 };

    for (int i = 0; i < 6; ++i) {
        char buf[80] = {0};
        int col = getColor4(0, 222, 222, 222);

        if (i == options_selected) {
            col = getColor4(0, 555, 555, 555);
            snprintf(buf, sizeof(buf), "> %s <", lines[i]);
        } else {
            snprintf(buf, sizeof(buf), "%s", lines[i]);
        }

        int optLen = strlen(buf);
        font_draw(buf, optLen, screen, (screen->w - optLen * 8) / 2, (yo + 2 + i * 2) * 8, col);
    }
}

void optionsmenu_tick(void) {
    if (up.clicked) {
        --options_selected;
        sound_play(SND_SELECT);
    }
    if (down.clicked) {
        ++options_selected;
        sound_play(SND_SELECT);
    }

    if (options_selected < 0) options_selected = 0;
    if (options_selected > 5) options_selected = 5;

    // Left / Right controls
    if (left.clicked) {
        if (options_selected == 0) {
            g_aspectRatio = 1 - g_aspectRatio;
            game_screen.w = (g_aspectRatio == 1) ? 426 : 320;
            game_lightScreen.w = game_screen.w;
            if (prevBuf) memset(prevBuf, 0xFF, sizeof(int) * 426 * 240);
            sound_play(SND_CONFIRM);
        } else if (options_selected == 1) {
            lang_next();
            sound_play(SND_CONFIRM);
        } else if (options_selected == 2) {
            g_buttonPrompts = 1 - g_buttonPrompts;
            sound_play(SND_CONFIRM);
        } else if (options_selected == 3) {
            if (g_sfxVolume > 0) --g_sfxVolume;
            sound_play(SND_MONSTERHURT);
        } else if (options_selected == 4) {
            if (g_uiVolume > 0) --g_uiVolume;
            sound_play(SND_SELECT);
        }
    }

    if (right.clicked) {
        if (options_selected == 0) {
            g_aspectRatio = 1 - g_aspectRatio;
            game_screen.w = (g_aspectRatio == 1) ? 426 : 320;
            game_lightScreen.w = game_screen.w;
            if (prevBuf) memset(prevBuf, 0xFF, sizeof(int) * 426 * 240);
            sound_play(SND_CONFIRM);
        } else if (options_selected == 1) {
            lang_next();
            sound_play(SND_CONFIRM);
        } else if (options_selected == 2) {
            g_buttonPrompts = 1 - g_buttonPrompts;
            sound_play(SND_CONFIRM);
        } else if (options_selected == 3) {
            if (g_sfxVolume < 10) ++g_sfxVolume;
            sound_play(SND_MONSTERHURT);
        } else if (options_selected == 4) {
            if (g_uiVolume < 10) ++g_uiVolume;
            sound_play(SND_SELECT);
        }
    }

    if (attack.clicked || menu.clicked) {
        if (options_selected == 0) {
            g_aspectRatio = 1 - g_aspectRatio;
            game_screen.w = (g_aspectRatio == 1) ? 426 : 320;
            game_lightScreen.w = game_screen.w;
            if (prevBuf) memset(prevBuf, 0xFF, sizeof(int) * 426 * 240);
            sound_play(SND_CONFIRM);
        } else if (options_selected == 1) {
            lang_next();
            sound_play(SND_CONFIRM);
        } else if (options_selected == 2) {
            g_buttonPrompts = 1 - g_buttonPrompts;
            sound_play(SND_CONFIRM);
        } else if (options_selected == 3) {
            g_sfxVolume = (g_sfxVolume >= 10) ? 0 : (g_sfxVolume + 2);
            sound_play(SND_MONSTERHURT);
        } else if (options_selected == 4) {
            g_uiVolume = (g_uiVolume >= 10) ? 0 : (g_uiVolume + 2);
            sound_play(SND_SELECT);
        } else if (options_selected == 5) {
            sound_play(SND_CONFIRM);
            game_set_menu(menu_parent ? menu_parent : mid_TITLE);
        }
    }
}
