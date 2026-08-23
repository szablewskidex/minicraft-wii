#include "sign.h"
#include "player.h"
#include "../game.h"
#include "../gfx/color.h"
#include "../sound.h"
#include <string.h>

void sign_create(Sign* sign){
	char* name = malloc(strlen("Sign") + 1);
	strcpy(name, "Sign");

	furniture_create((Furniture *) sign, name);

	sign->furniture.entity.type = SIGN;
	sign->furniture.col = getColor4(-1, 100, 321, 555);
	sign->furniture.sprite = 13; // (26 / 2)
	sign->signIndex = 0;
}

char sign_use(Sign* sign, struct _Player* player, int attackDir){
	sound_play(SND_CONFIRM);
	return 1;
}

void sign_free(Sign* sign){
	furniture_free((Furniture *) sign);
}
