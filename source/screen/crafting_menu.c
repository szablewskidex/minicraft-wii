#include "../gfx/font.h"
#include "../gfx/color.h"
#include "crafting_menu.h"
#include "../crafting/recipe.h"
#include "../game.h"
#include "../inputhandler.h"

#include "../sound.h"

const menu_vt craftingmenu_vt = {
	&craftingmenu_tick,
	&craftingmenu_render,
	&craftingmenu_init
};

ArrayList* crmenu_recipes = 0;
static int selected = 0;
#include "../lang.h"

void craftingmenu_tick(){

	if(menu.clicked) game_set_menu(0);
	if(up.clicked) {
		--selected;
		sound_play(SND_SELECT);
	}
	if(down.clicked) {
		++selected;
		sound_play(SND_SELECT);
	}

	int len = crmenu_recipes->size;
	if(len == 0) selected = 0;
	if(selected < 0) selected += len;
	if(selected >= len) selected -= len;

	if(attack.clicked && len > 0){
		Recipe* r = crmenu_recipes->elements[selected];
		recipe_checkCanCraft(r, game_player);

		if(r->canCraft){
			recipe_deductCost(r, game_player);
			recipe_craft(r, game_player);
			sound_play(SND_CRAFT);
		}

		for(int i = 0; i < crmenu_recipes->size; ++i){
			recipe_checkCanCraft(crmenu_recipes->elements[i], game_player);
		}
	}
}

static int _cmpRec(const void* recipe, const void* recipe2){
	Recipe* r1 = *(Recipe**)recipe;
	Recipe* r2 = *(Recipe**)recipe2;

	if(r1->canCraft && !r2->canCraft) return -1;
	if(!r1->canCraft && r2->canCraft) return 1;
	return 0;
}

void craftingmenu_init(){
	selected = 0;

	for(int i = 0; i < crmenu_recipes->size; ++i){
		recipe_checkCanCraft(crmenu_recipes->elements[i], game_player);
	}
	qsort(crmenu_recipes->elements, crmenu_recipes->size, sizeof(*crmenu_recipes->elements), _cmpRec);
}
void craftingmenu_render(Screen* screen){
	const char* s1 = _T(STR_HAVE);
	const char* s2 = _T(STR_COST);
	const char* s3 = _T(STR_CRAFTING);
	char buf[64] = {0};
	font_renderFrame(screen, (char*)s1, strlen(s1), 12, 1, 19, 3);
	font_renderFrame(screen, (char*)s2, strlen(s2), 12, 4, 19, 11);
	font_renderFrame(screen, (char*)s3, strlen(s3), 0, 1, 11, 11);
	menu_render_item_list(screen, 0, 1, 11, 11, crmenu_recipes, selected, recipe_renderInventory);
	if(crmenu_recipes->size > 0){
		Recipe* recipe = crmenu_recipes->elements[selected];
		Item* result = &recipe->resultTemplate;
		int hasResultItems = inventory_count(&game_player->inventory, result);
		int xo = 13*8;
		render_screen(screen, xo, 2*8, item_getSprite(result), item_getColor(result), 0);
		sprintf(buf, "%d\00", hasResultItems);
		font_draw(buf, strlen(buf), screen, xo + 8, 2*8, getColor4(-1, 555, 555, 555));

		for(int i = 0; i < recipe->costs.size; ++i){
			Item* item = recipe->costs.elements[i];
			int yo = (5+i)*8;
			render_screen(screen, xo, yo, item_getSprite(item), item_getColor(item), 0);
			int requiredAmt = 1;
			if(item->id == RESOURCE){
				requiredAmt = item->add.resource.count;
			}

			int has = inventory_count(&game_player->inventory, item);
			int color = getColor4(-1, 555, 555, 555);
			if(has < requiredAmt) color = getColor4(-1, 222, 222, 222);

			if(has > 99) has = 99;
			sprintf(buf, "%d/%d\00", requiredAmt, has);
			font_draw(buf, strlen(buf), screen, xo+8, yo, color);
		}
	}
}
