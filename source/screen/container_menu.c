#include "menu.h"
#include "container_menu.h"
#include "../game.h"
#include "../inputhandler.h"
#include "../entity/inventory.h"
#include "../gfx/font.h"
#include "../item/item.h"

#include <string.h>

const menu_vt containermenu_vt = {
	&containermenu_tick,
	&containermenu_render,
	&containermenu_init
};

static int window = 0;
static int oSelected = 0;
static int selected = 0;

Inventory* contmenu_container = 0;
char contmenu_title[64] = {0};

void containermenu_tick(){
	if(menu.clicked) game_set_menu(0);

	if(left.clicked){
		window = 0;
		int tmp = selected;
		selected = oSelected;
		oSelected = tmp;
	}

	if(right.clicked){
		window = 1;
		int tmp = selected;
		selected = oSelected;
		oSelected = tmp;
	}

	Inventory* i = window == 1 ? &game_player->inventory : contmenu_container;
	Inventory* i2 = window == 0 ? &game_player->inventory : contmenu_container;

	int len = i->items.size;
	if(selected < 0) selected = 0;
	if(selected >= len) selected = len-1;

	if(up.clicked) --selected;
	if(down.clicked) ++selected;

	if(len == 0) selected = 0;
	if(selected < 0) selected += len;
	if(selected >= len) selected -= len;

	if(attack.clicked && len > 0){
		Item* item = arraylist_removeId(&i->items, selected);
		inventory_addItemIntoSlot(i2, oSelected, item);


		free(item);

		if(selected >= i->items.size) selected = i->items.size - 1;
	}
}
void containermenu_init(){
	window = oSelected = selected = 0;
}
#include "../lang.h"

void containermenu_render(Screen* screen){
	int box_w = (screen->w >= 400) ? 19 : 16;
	int box_h = 16;
	int x_margin = (screen->w >= 400) ? ((screen->w / 8 - (box_w * 2 + 2)) / 2) : 1;
	if (x_margin < 1) x_margin = 1;

	int box1_x0 = x_margin;
	int box1_x1 = box1_x0 + box_w;
	int box2_x0 = box1_x1 + 1;
	int box2_x1 = box2_x0 + box_w;

	const char* t_title = lang_translate_item(contmenu_title);
	if (!t_title || strlen(t_title) == 0) t_title = contmenu_title;

	font_renderFrame(screen, (char*)t_title, strlen(t_title), box1_x0, 1, box1_x1, box_h);
	menu_render_item_list(screen, box1_x0, 1, box1_x1, box_h, &contmenu_container->items, window == 0 ? selected : -oSelected - 1, item_renderInventory);

	const char* s = _T(STR_INVENTORY);
	font_renderFrame(screen, (char*)s, strlen(s), box2_x0, 1, box2_x1, box_h);
	menu_render_item_list(screen, box2_x0, 1, box2_x1, box_h, &game_player->inventory.items, window == 1 ? selected : -oSelected - 1, item_renderInventory);
}
