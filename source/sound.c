#include "sound.h"
#include "sound_data.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef USE_SDL1
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

static Mix_Chunk* chunks[9] = {0};
static const uint8_t* sound_ptrs[9] = {
    sound_select, sound_confirm, sound_craft, sound_pickup,
    sound_playerhurt, sound_monsterhurt, sound_death, sound_bossdeath, sound_explode
};
static const size_t sound_sizes[9] = {
    sound_select_size, sound_confirm_size, sound_craft_size, sound_pickup_size,
    sound_playerhurt_size, sound_monsterhurt_size, sound_death_size, sound_bossdeath_size, sound_explode_size
};

static int sound_ready = 0;

void sound_init(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0) {
        Mix_AllocateChannels(16);
        for (int i = 0; i < 9; ++i) {
            chunks[i] = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_ptrs[i], (int)sound_sizes[i]), 1);
        }
        sound_ready = 1;
        printf("SDL_mixer initialized successfully with 16 channels!\n");
    } else {
        printf("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
    }
}

static int sound_last_played[9] = {0};
static int sound_frame_counter = 0;

void sound_tick(void) {
    sound_frame_counter++;
}

void sound_play(SoundEffect effect) {
    if (!sound_ready) return;
    if (effect < 0 || effect >= 9) return;

    // Prevent identical sound spamming within 3 frames
    if (sound_frame_counter - sound_last_played[effect] < 3) {
        return;
    }
    sound_last_played[effect] = sound_frame_counter;

    if (chunks[effect]) {
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
