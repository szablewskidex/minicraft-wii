#include "exporb.h"
#include "player.h"
#include "../game.h"
#include "../gfx/color.h"
#include "../sound.h"
#include "particle/textparticle.h"
#include <math.h>

void exporb_create(ExpOrb* orb, int x, int y, int expValue) {
    entity_create(&orb->entity);
    orb->entity.type = EXPORB;
    orb->entity.x = orb->xx = x;
    orb->entity.y = orb->yy = y;
    orb->entity.xr = 3;
    orb->entity.yr = 3;
    orb->expValue = expValue;
    orb->zz = 2;
    orb->xa = (random_next_float(&orb->entity.random) - 0.5f) * 1.2f;
    orb->ya = (random_next_float(&orb->entity.random) - 0.5f) * 1.2f;
    orb->za = random_next_float(&orb->entity.random) * 0.8f + 1.2f;
    orb->time = 0;
    orb->lifeTime = 60 * 30; // 30 seconds
}

void exporb_tick(ExpOrb* orb) {
    ++orb->time;
    if (orb->time >= orb->lifeTime) {
        entity_remove(&orb->entity);
        return;
    }

    // Magnet to player if nearby
    if (game_player && game_player->mob.entity.level == orb->entity.level) {
        int xd = game_player->mob.entity.x - orb->entity.x;
        int yd = game_player->mob.entity.y - orb->entity.y;
        int dist2 = xd * xd + yd * yd;

        if (dist2 < (36 * 36)) {
            // Fly towards player
            orb->xa += (xd > 0 ? 0.3f : -0.3f);
            orb->ya += (yd > 0 ? 0.3f : -0.3f);
        }

        // Collect when touching player
        if (dist2 < (8 * 8)) {
            player_addExp(game_player, orb->expValue);
            game_player->score += orb->expValue;
            sound_play(SND_PICKUP);
            entity_remove(&orb->entity);
            return;
        }
    }

    orb->xx += orb->xa;
    orb->yy += orb->ya;
    orb->zz += orb->za;

    if (orb->zz < 0) {
        orb->zz = 0;
        orb->za *= -0.5f;
        orb->xa *= 0.6f;
        orb->ya *= 0.6f;
    }

    orb->za -= 0.15f;

    int ox = orb->entity.x;
    int oy = orb->entity.y;
    int nx = (int)orb->xx;
    int ny = (int)orb->yy;
    int expectedx = nx - orb->entity.x;
    int expectedy = ny - orb->entity.y;
    entity_move(&orb->entity, expectedx, expectedy);

    int gotx = orb->entity.x - ox;
    int goty = orb->entity.y - oy;
    orb->xx += gotx - expectedx;
    orb->yy += goty - expectedy;
}

void exporb_render(ExpOrb* orb, Screen* screen) {
    if (orb->time >= orb->lifeTime - 6 * 20) {
        if ((orb->time / 6) % 2 == 0) return;
    }

    int x = orb->entity.x;
    int y = orb->entity.y;

    // Glowing flashing colors (green, cyan, yellow, bright white)
    int phase = (orb->time / 4) % 4;
    int col = getColor4(-1, 050, 252, 555);
    if (phase == 1) col = getColor4(-1, 055, 255, 555);
    if (phase == 2) col = getColor4(-1, 550, 552, 555);
    if (phase == 3) col = getColor4(-1, 525, 545, 555);

    // Drop shadow on ground
    render_screen(screen, x - 4, y - 4, 2 + 12 * 32, getColor4(-1, 000, 000, 000), 0);
    // Glowing bouncing orb
    render_screen(screen, x - 4, y - 4 - (int)orb->zz, 2 + 12 * 32, col, 0);
}

void exporb_touchedBy(ExpOrb* orb, Entity* entity) {
    if (entity->type == PLAYER) {
        Player* player = (Player*)entity;
        player_addExp(player, orb->expValue);
        player->score += orb->expValue;
        sound_play(SND_PICKUP);
        entity_remove(&orb->entity);
    }
}
