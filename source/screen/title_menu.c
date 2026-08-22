#include "menu.h"
#include "../inputhandler.h"
#include "title_menu.h"
#include "slot_menu.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../game.h"
#include "../save.h"
#include "../sound.h"
#include "../lang.h"

#include <string.h>
#include <stdio.h>

int titlemenu_selected = 0;
static int has_save = 0;

const menu_vt titlemenu_vt = {
	&titlemenu_tick,
	&titlemenu_render,
	&titlemenu_init
};


void titlemenu_init() {
	has_save = save_exists(NULL);
	titlemenu_selected = 0;
}


void titlemenu_render(Screen* screen) {
	clear_screen(screen, 0);

	int h = 2;
	int w = 13;
    int yo = 20;
    int xo = (screen->w - w * 8) / 2;
    
	int titleColor = getColor4(0, 10, 131, 551);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			render_screen(screen, xo + x * 8, yo + y * 8, x + (y + 6) * 32, titleColor, 0);
		}
	}

	int count = has_save ? 6 : 5;
	for (int i = 0; i < count; ++i) {
		char optBuf[64] = {0};
		const char* text = "";

		if (has_save) {
			if (i == 0) text = _T(STR_CONTINUE);
			else if (i == 1) text = _T(STR_NEW_GAME);
			else if (i == 2) text = _T(STR_OPTIONS);
			else if (i == 3) text = _T(STR_HOW_TO_PLAY);
			else if (i == 4) text = _T(STR_ABOUT);
			else if (i == 5) text = _T(STR_EXIT);
		} else {
			if (i == 0) text = _T(STR_START_GAME);
			else if (i == 1) text = _T(STR_OPTIONS);
			else if (i == 2) text = _T(STR_HOW_TO_PLAY);
			else if (i == 3) text = _T(STR_ABOUT);
			else if (i == 4) text = _T(STR_EXIT);
		}

		int col = getColor4(0, 222, 222, 222);
		if (i == titlemenu_selected) {
			col = getColor4(0, 555, 555, 555);
			snprintf(optBuf, sizeof(optBuf), "> %s <", text);
		} else {
			snprintf(optBuf, sizeof(optBuf), "%s", text);
		}

		int optionLength = strlen(optBuf);
		font_draw(optBuf, optionLength, screen, (screen->w - optionLength * 8) / 2, (6 + i) * 10, col);
	}

	const char* help = _T(STR_HELP_MSG);
	int helpLen = strlen(help);
	font_draw((char*)help, helpLen, screen, (screen->w - helpLen * 8) / 2, screen->h - 12, getColor4(0, 111, 111, 111));
}


void titlemenu_tick() {
	int count = has_save ? 6 : 5;

	if (up.clicked) {
		--titlemenu_selected;
		sound_play(SND_SELECT);
	}
	if (down.clicked) {
		++titlemenu_selected;
		sound_play(SND_SELECT);
	}

	if (titlemenu_selected < 0) titlemenu_selected = 0;
	if (titlemenu_selected >= count) titlemenu_selected = count - 1;

	if (attack.clicked || menu.clicked) {
		sound_play(SND_CONFIRM);

		if (has_save) {
			if (titlemenu_selected == 0) {
				// Continue -> Slot Selection
				slotmenu_mode = 0;
				game_set_menu(mid_SLOT_SELECT);
			} else if (titlemenu_selected == 1) {
				// New Game -> Slot Selection
				slotmenu_mode = 1;
				game_set_menu(mid_SLOT_SELECT);
			} else if (titlemenu_selected == 2) {
				// Options
				menu_parent = mid_TITLE;
				game_set_menu(mid_OPTIONS);
			} else if (titlemenu_selected == 3) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_INSTRUCTIONS);
			} else if (titlemenu_selected == 4) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_ABOUT);
			} else if (titlemenu_selected == 5) {
				exit(0);
			}
		} else {
			if (titlemenu_selected == 0) {
				// New Game / Start Game -> Slot Selection
				slotmenu_mode = 1;
				game_set_menu(mid_SLOT_SELECT);
			} else if (titlemenu_selected == 1) {
				// Options
				menu_parent = mid_TITLE;
				game_set_menu(mid_OPTIONS);
			} else if (titlemenu_selected == 2) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_INSTRUCTIONS);
			} else if (titlemenu_selected == 3) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_ABOUT);
			} else if (titlemenu_selected == 4) {
				exit(0);
			}
		}
	}
}
