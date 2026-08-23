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

    int xp = orb->entity.x - screen->xOffset;
    int yp = orb->entity.y - (int)orb->zz - screen->yOffset;

    // Glowing vibrant Minecraft EXP colors: emerald green, lime, yellow, cyan
    int phase = (orb->time / 3) % 4;
    int coreCol = getColor(555); // Bright white core
    int edgeCol = getColor(050); // Vibrant green
    if (phase == 1) edgeCol = getColor(250); // Lime green
    if (phase == 2) edgeCol = getColor(550); // Yellow
    if (phase == 3) edgeCol = getColor(055); // Cyan

    // Render a small, glowing 3x3 diamond orb
    if (xp >= 1 && xp < screen->w - 1 && yp >= 1 && yp < screen->h - 1) {
        screen->pixels[xp + (yp - 1) * screen->w] = edgeCol;
        screen->pixels[(xp - 1) + yp * screen->w] = edgeCol;
        screen->pixels[xp + yp * screen->w] = coreCol;
        screen->pixels[(xp + 1) + yp * screen->w] = edgeCol;
        screen->pixels[xp + (yp + 1) * screen->w] = edgeCol;
    }
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
