#ifndef SCREEN_PAUSE_MENU_H_
#define SCREEN_PAUSE_MENU_H_

#include "../gfx/screen.h"
#include "menu.h"

extern const menu_vt pausemenu_vt;

void pausemenu_init(void);
void pausemenu_tick(void);
void pausemenu_render(Screen* screen);

#endif /* SCREEN_PAUSE_MENU_H_ */
