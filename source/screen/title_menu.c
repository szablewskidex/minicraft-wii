#include "menu.h"
#include "../inputhandler.h"
#include "title_menu.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../game.h"
#include "../save.h"

#include <string.h>

static char continue_game[] = "Continue";
static char start_game[] = "Start game";
static char new_game[] = "New Game";
static char how_to_play[] = "How to play";
static char about[] = "About";

static char s_continue_game[] = "> Continue <";
static char s_start_game[] = "> Start Game <";
static char s_new_game[] = "> New Game <";
static char s_how_to_play[] = "> How to play <";
static char s_about[] = "> About <";

static char small_help_msg[] = "(D-Pad, 2 to Action, 1 for Menu)";

static char* titlemenu_options[4];
static char* s_titlemenu_options[4];
static int titlemenu_count = 3;
static int has_save = 0;

int titlemenu_selected = 0;

const menu_vt titlemenu_vt = {
	&titlemenu_tick,
	&titlemenu_render,
	&titlemenu_init
};


void titlemenu_init() {
	has_save = save_exists(NULL);
	if (has_save) {
		titlemenu_options[0] = continue_game;
		titlemenu_options[1] = new_game;
		titlemenu_options[2] = how_to_play;
		titlemenu_options[3] = about;

		s_titlemenu_options[0] = s_continue_game;
		s_titlemenu_options[1] = s_new_game;
		s_titlemenu_options[2] = s_how_to_play;
		s_titlemenu_options[3] = s_about;
		titlemenu_count = 4;
	} else {
		titlemenu_options[0] = start_game;
		titlemenu_options[1] = how_to_play;
		titlemenu_options[2] = about;

		s_titlemenu_options[0] = s_start_game;
		s_titlemenu_options[1] = s_how_to_play;
		s_titlemenu_options[2] = s_about;
		titlemenu_count = 3;
	}
	titlemenu_selected = 0;
}


void titlemenu_render(Screen* screen) {
	clear_screen(screen, 0);

	int h = 2;
	int w = 13;
    int yo = 24;
    int xo = (screen->w - w * 8) / 2;
    
	int titleColor = getColor4(0, 10, 131, 551);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			render_screen(screen, xo + x * 8, yo + y * 8, x + (y + 6) * 32, titleColor, 0);
		}
	}

	for (int i = 0; i < titlemenu_count; ++i) {
		char* option = titlemenu_options[i];
		int col = getColor4(0, 222, 222, 222);

		if (i == titlemenu_selected) {
			col = getColor4(0, 555, 555, 555);
			option = s_titlemenu_options[i];
		}

		int optionLength = strlen(option);
		font_draw(option, optionLength, screen, (screen->w - optionLength * 8) / 2, (8 + i) * 8, col);
	}

	font_draw(small_help_msg, strlen(small_help_msg), screen, 0, screen->h - 8, getColor4(0, 111, 111, 111));
}


void titlemenu_tick() {
	if (up.clicked) --titlemenu_selected;
	if (down.clicked) ++titlemenu_selected;

	if (titlemenu_selected < 0) titlemenu_selected = 0;
	if (titlemenu_selected >= titlemenu_count) titlemenu_selected = titlemenu_count - 1;

	if (attack.clicked || menu.clicked) {
		if (has_save) {
			if (titlemenu_selected == 0) {
				// Continue / Load
				if (!load_game(NULL)) {
					// If load failed, start fresh
					isingame = 1;
					game_reset();
					game_set_menu(0);
				}
			} else if (titlemenu_selected == 1) {
				// New Game
				isingame = 1;
				game_reset();
				game_set_menu(0);
			} else if (titlemenu_selected == 2) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_INSTRUCTIONS);
			} else if (titlemenu_selected == 3) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_ABOUT);
			}
		} else {
			if (titlemenu_selected == 0) {
				isingame = 1;
				game_reset();
				game_set_menu(0);
			} else if (titlemenu_selected == 1) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_INSTRUCTIONS);
			} else if (titlemenu_selected == 2) {
				menu_parent = mid_TITLE;
				game_set_menu(mid_ABOUT);
			}
		}
	}
}
