#include "itemids.h"
#include "resourceitem.h"
#include "resource/resource.h"
#include <stdlib.h>
#include "../gfx/font.h"
#include <string.h>
#include "../gfx/color.h"

void resourceitem_create(Item* item, Resource* resource){
	item->id = RESOURCE;
	item->add.resource.resource = resource;
	item->add.resource.count = 1;
	item->add.resource.maxDur = 0;
	item->add.resource.dur = 0;

	if (resource == &fishingRod) {
		item->add.resource.maxDur = 30;
		item->add.resource.dur = 30;
	} else if (resource == &shears) {
		item->add.resource.maxDur = 40;
		item->add.resource.dur = 40;
	} else if (resource == &bow) {
		item->add.resource.maxDur = 50;
		item->add.resource.dur = 50;
	}
}

void resourceitem_create_cnt(Item* item, Resource* resource, int count){
	item->id = RESOURCE;
	item->add.resource.resource = resource;
	item->add.resource.count = count;
	item->add.resource.maxDur = 0;
	item->add.resource.dur = 0;

	if (resource == &fishingRod) {
		item->add.resource.maxDur = 30;
		item->add.resource.dur = 30;
	} else if (resource == &shears) {
		item->add.resource.maxDur = 40;
		item->add.resource.dur = 40;
	} else if (resource == &bow) {
		item->add.resource.maxDur = 50;
		item->add.resource.dur = 50;
	}
}

int resourceitem_getColor(Item* item){
	return item->add.resource.resource->color;
}

int resourceitem_getSprite(Item* item){
	return item->add.resource.resource->sprite;
}

void resourceitem_renderIcon(Item* item, Screen* screen, int x, int y){
	render_screen(screen, x, y, item->add.resource.resource->sprite, item->add.resource.resource->color, 0);
	if (item->add.resource.maxDur > 0 && item->add.resource.dur < item->add.resource.maxDur) {
		int filled = (item->add.resource.dur * 7) / item->add.resource.maxDur;
		if (filled < 1) filled = 1;
		int barCol = getColor(050); // Green
		if (item->add.resource.dur <= item->add.resource.maxDur / 3) barCol = getColor(500); // Red
		else if (item->add.resource.dur <= item->add.resource.maxDur * 2 / 3) barCol = getColor(550); // Yellow

		for (int px = 0; px < 7; ++px) {
			int sx = x + 1 + px;
			int sy = y + 7;
			if (sx >= 0 && sx < screen->w && sy >= 0 && sy < screen->h) {
				screen->pixels[sx + sy * screen->w] = (px < filled) ? barCol : getColor(000);
			}
		}
	}
}

#include "../lang.h"

void resourceitem_renderInventory(Item* item, Screen* screen, int x, int y){
	resourceitem_renderIcon(item, screen, x, y);
	const char* tName = lang_translate_item(item->add.resource.resource->name);

	if (item->add.resource.maxDur > 0) {
		font_draw((char*)tName, strlen(tName), screen, x + 10, y, getColor4(-1, 555, 555, 555));
	} else {
		int cc = item->add.resource.count;
		if (cc > 999) cc = 999;
		char num[16] = {0};
		snprintf(num, sizeof(num), "%-3d ", cc);
		font_draw(num, strlen(num), screen, x + 8, y, getColor4(-1, 444, 444, 444));
		font_draw((char*)tName, strlen(tName), screen, x + 8 + (int)strlen(num) * 8, y, getColor4(-1, 555, 555, 555));
	}
}

char* resourceitem_getName(Item* item){
	return (char*)lang_translate_item(item->add.resource.resource->name);
}

char resourceitem_interactOn(Item* item, TileID tile, Level* level, int xt, int yt, Player* player, int attackDir){
	char b = resource_interactOn(item->add.resource.resource, tile, level, xt, yt, player, attackDir);
	if(b){
		if (item->add.resource.maxDur > 0) {
			--item->add.resource.dur;
		} else {
			--item->add.resource.count;
		}
		return 1;
	}
	return 0;
}

char resourceitem_isDepleted(Item* item){
	if (item->add.resource.maxDur > 0) {
		return item->add.resource.dur <= 0 || item->add.resource.count <= 0;
	}
	return item->add.resource.count <= 0;
}
