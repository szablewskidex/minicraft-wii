#ifndef SCREEN_SLOT_MENU_H_
#define SCREEN_SLOT_MENU_H_

#include "../gfx/screen.h"
#include "menu.h"

extern const menu_vt slotmenu_vt;
extern int slotmenu_mode; // 0 = Load Game, 1 = New Game

void slotmenu_init(void);
void slotmenu_tick(void);
void slotmenu_render(Screen* screen);

#endif /* SCREEN_SLOT_MENU_H_ */
