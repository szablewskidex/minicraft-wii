#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "menu.h"
#include "../gfx/screen.h"

extern const menu_vt optionsmenu_vt;

void optionsmenu_init(void);
void optionsmenu_render(Screen* screen);
void optionsmenu_tick(void);

#endif // OPTIONS_MENU_H
