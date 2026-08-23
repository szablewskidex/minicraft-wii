#include "barrel.h"
#include "player.h"
#include "inventory.h"
#include "../screen/container_menu.h"
#include "../game.h"
#include "../gfx/color.h"
#include <string.h>

void barrel_create(Barrel* barrel){
	char* name = malloc(strlen("Barrel") + 1);
	strcpy(name, "Barrel");

	furniture_create((Furniture *) barrel, name);

	barrel->furniture.entity.type = BARREL;
	barrel->furniture.col = getColor4(-1, 100, 321, 431);
	barrel->furniture.sprite = 12; // (24 / 2)

	inventory_create(&barrel->inventory);
}

char barrel_use(Barrel* barrel, struct _Player* player, int attackDir){
	strcpy(contmenu_title, "Barrel");
	contmenu_container = &barrel->inventory;
	game_set_menu(mid_CONTAINER);
	return 1;
}

void barrel_free(Barrel* barrel){
	furniture_free((Furniture *) barrel);
	inventory_free(&barrel->inventory);
}
