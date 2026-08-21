#include "won_menu.h"
#include "../gfx/screen.h"
#include "../gfx/color.h"
#include "../gfx/font.h"
#include "../inputhandler.h"
#include "../game.h"

#include <string.h>

static int inputDelay = 60;


const menu_vt wonmenu_vt = {
	wonmenu_tick,
	wonmenu_render,
	wonmenu_init
};


void wonmenu_init(){
	inputDelay = 60;
}


#include "../sound.h"
#include "../lang.h"

void wonmenu_render(Screen* screen) {
	const char* msg = _T(STR_YOU_WON);
	char timeString[256];
	font_renderFrame(screen, "", 0, 1, 3, 18, 9);
	font_draw((char*)msg, strlen(msg), screen, 2 * 8, 4 * 8, getColor4(-1, 555, 555, 555));

	int seconds = game_gameTime / 60;
	int minutes = seconds / 60;
	int hours = minutes / 60;
	minutes %= 60;
	seconds %= 60;

	if (hours > 0) {
		if (seconds < 10) {
			sprintf(timeString, "%dh0%dm", hours, minutes);
		} else {
			sprintf(timeString, "%dh%dm", hours, minutes);
		}
	} else {
		if (seconds < 10) {
			sprintf(timeString, "%dm 0%ds", minutes, seconds);
		} else {
			sprintf(timeString, "%dm %ds", minutes, seconds);
		}
	}

	char time[32];
	char scor[32];
	snprintf(time, sizeof(time), "%s:", _T(STR_TIME));
	snprintf(scor, sizeof(scor), "%s:", _T(STR_SCORE));
	const char* prec = _T(STR_PRESS_TO_RESTART);

	char score[32];
	sprintf(score, "%d", game_player->score);
	font_draw(time, strlen(time), screen, 2 * 8, 5 * 8, getColor4(-1, 555, 555, 555));
	font_draw(timeString, strlen(timeString), screen, (2 + strlen(time) + 1) * 8, 5 * 8, getColor4(-1, 550, 550, 550));
	font_draw(scor, strlen(scor), screen, 2 * 8, 6 * 8, getColor4(-1, 555, 555, 555));
	font_draw(score, strlen(score), screen, (2 + strlen(scor) + 1) * 8, 6 * 8, getColor4(-1, 550, 550, 550));
	font_draw((char*)prec, strlen(prec), screen, 2 * 8, 8 * 8, getColor4(-1, 333, 333, 333));
}


void wonmenu_tick() {
	if (inputDelay > 0) {
		--inputDelay;
	} else if (attack.clicked || menu.clicked) {
		sound_play(SND_CONFIRM);
		game_set_menu(mid_TITLE);
	}
}
