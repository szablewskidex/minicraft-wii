#ifndef SCREEN_NEWGAME_MENU_H_
#define SCREEN_NEWGAME_MENU_H_

#include "../gfx/screen.h"
#include "menu.h"

extern const menu_vt newgamemenu_vt;

void newgamemenu_init(void);
void newgamemenu_tick(void);
void newgamemenu_render(Screen* screen);

#endif /* SCREEN_NEWGAME_MENU_H_ */
