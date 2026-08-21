#include "bed.h"
#include "../gfx/color.h"
#include "../game.h"
#include "../sound.h"
#include "player.h"
#include <string.h>

void bed_create(Bed* bed){
    furniture_create((Furniture*)bed, "Bed");
    bed->col = getColor4(-1, 100, 421, 555); // Wooden frame with white cloth
    bed->sprite = 4;
    bed->entity.type = BED;
}

char bed_use(Bed* bed, struct _Player* player, int attackDir){
    (void)bed;
    (void)attackDir;
    
    // Check if on surface and night time
    int dayTime = game_gameTime % 24000;
    if (dayTime > 12000 && dayTime < 22000) {
        // Sleep and advance to morning!
        game_gameTime += (24000 - dayTime);
        player->mob.health = player->mob.maxHealth;
        player->stamina = player->maxStamina;
        sound_play(SND_CONFIRM);
        return 1;
    } else {
        // Daytime: rest a bit to heal 1 health
        if (player->mob.health < player->mob.maxHealth) {
            player->mob.health += 1;
            sound_play(SND_CONFIRM);
            return 1;
        }
    }
    return 0;
}
