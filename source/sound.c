#include "sound.h"
#include "sound_data.h"

#ifdef USE_SDL1
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif
#include <stdio.h>

static Mix_Chunk* chunks[9] = {0};
static int sound_ready = 0;

void sound_init(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        return;
    }
    Mix_AllocateChannels(16);

    chunks[SND_SELECT]      = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_select, (int)sound_select_size), 1);
    chunks[SND_CONFIRM]     = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_confirm, (int)sound_confirm_size), 1);
    chunks[SND_CRAFT]       = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_craft, (int)sound_craft_size), 1);
    chunks[SND_PICKUP]      = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_pickup, (int)sound_pickup_size), 1);
    chunks[SND_PLAYERHURT]  = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_playerhurt, (int)sound_playerhurt_size), 1);
    chunks[SND_MONSTERHURT] = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_monsterhurt, (int)sound_monsterhurt_size), 1);
    chunks[SND_DEATH]       = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_death, (int)sound_death_size), 1);
    chunks[SND_BOSSDEATH]   = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_bossdeath, (int)sound_bossdeath_size), 1);
    chunks[SND_EXPLODE]     = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_explode, (int)sound_explode_size), 1);

    sound_ready = 1;
    printf("Minicraft sound system initialized successfully!\n");
}

void sound_play(SoundEffect effect) {
    if (!sound_ready) return;
    if (effect >= 0 && effect < 9 && chunks[effect]) {
        Mix_PlayChannel(-1, chunks[effect], 0);
    }
}

void sound_cleanup(void) {
    if (!sound_ready) return;
    for (int i = 0; i < 9; ++i) {
        if (chunks[i]) Mix_FreeChunk(chunks[i]);
        chunks[i] = NULL;
    }
    Mix_CloseAudio();
    sound_ready = 0;
}
