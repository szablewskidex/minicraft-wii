#include "sound.h"
#include "sound_data.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __wii__
#include <asndlib.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
static Mix_Chunk* chunks[9] = {0};
#endif

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
#ifdef __wii__
    ASND_Init();
    ASND_Pause(0);
    sound_ready = 1;
    printf("Hardware ASND audio engine initialized (0ms latency)!\n");
#else
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == 0) {
        Mix_AllocateChannels(16);
        for (int i = 0; i < 9; ++i) {
            chunks[i] = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_ptrs[i], (int)sound_sizes[i]), 1);
        }
        sound_ready = 1;
    }
#endif
}

void sound_play(SoundEffect effect) {
    if (!sound_ready) return;
    if (effect < 0 || effect >= 9) return;

#ifdef __wii__
    const uint8_t* raw = sound_ptrs[effect];
    size_t raw_size = sound_sizes[effect];
    if (raw_size <= 44) return;

    // Locate "data" chunk in WAV
    const uint8_t* pcm = raw + 44;
    int pcm_size = (int)raw_size - 44;
    for (size_t i = 12; i < 60 && i + 8 < raw_size; ++i) {
        if (raw[i] == 'd' && raw[i+1] == 'a' && raw[i+2] == 't' && raw[i+3] == 'a') {
            pcm = raw + i + 8;
            pcm_size = (int)(raw[i+4] | (raw[i+5] << 8) | (raw[i+6] << 16) | (raw[i+7] << 24));
            break;
        }
    }

    int format = (effect == SND_EXPLODE) ? VOICE_STEREO_16BIT_LE : VOICE_MONO_16BIT_LE;
    s32 voice = ASND_GetFirstUnusedVoice();
    if (voice >= 0) {
        ASND_SetVoice(voice, format, 44100, 0, (void*)pcm, pcm_size, 255, 255, NULL);
    }
#else
    if (chunks[effect]) {
        Mix_PlayChannel(-1, chunks[effect], 0);
    }
#endif
}

void sound_cleanup(void) {
    if (!sound_ready) return;
#ifdef __wii__
    ASND_Pause(1);
    ASND_End();
#else
    for (int i = 0; i < 9; ++i) {
        if (chunks[i]) Mix_FreeChunk(chunks[i]);
        chunks[i] = NULL;
    }
    Mix_CloseAudio();
#endif
    sound_ready = 0;
}
