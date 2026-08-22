#include "sound.h"
#include "sound_data.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef __wii__
#include <asndlib.h>
#include <malloc.h>
#include <ogc/cache.h>

static uint8_t* pcm_buffers[9] = {0};
static int pcm_sizes[9] = {0};
static int pcm_formats[9] = {0};
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

    for (int effect = 0; effect < 9; ++effect) {
        const uint8_t* raw = sound_ptrs[effect];
        size_t raw_size = sound_sizes[effect];
        if (raw_size <= 44) continue;

        const uint8_t* pcm = raw + 44;
        int pcm_size = (int)raw_size - 44;
        for (size_t i = 12; i < 60 && i + 8 < raw_size; ++i) {
            if (raw[i] == 'd' && raw[i+1] == 'a' && raw[i+2] == 't' && raw[i+3] == 'a') {
                pcm = raw + i + 8;
                pcm_size = (int)(raw[i+4] | (raw[i+5] << 8) | (raw[i+6] << 16) | (raw[i+7] << 24));
                break;
            }
        }

        // Must be aligned to 32 bytes and padded to 32 bytes for Wii DSP DMA
        int aligned_size = (pcm_size + 31) & ~31;
        uint8_t* buf = (uint8_t*)memalign(32, aligned_size);
        if (buf) {
            memset(buf, 0, aligned_size);
            memcpy(buf, pcm, pcm_size);
            DCFlushRange(buf, aligned_size);
            pcm_buffers[effect] = buf;
            pcm_sizes[effect] = aligned_size;
            pcm_formats[effect] = (effect == SND_EXPLODE) ? VOICE_STEREO_16BIT_LE : VOICE_MONO_16BIT_LE;
        }
    }

    sound_ready = 1;
    printf("Hardware ASND audio engine initialized with 32-byte aligned DSP buffers!\n");
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

static int sound_last_played[9] = {0};
static int sound_frame_counter = 0;

void sound_tick(void) {
    sound_frame_counter++;
}

void sound_play(SoundEffect effect) {
    if (!sound_ready) return;
    if (effect < 0 || effect >= 9) return;

    // Prevent the same sound from spamming within 3 frames
    if (sound_frame_counter - sound_last_played[effect] < 3) {
        return;
    }
    sound_last_played[effect] = sound_frame_counter;

#ifdef __wii__
    if (!pcm_buffers[effect]) return;

    s32 voice = ASND_GetFirstUnusedVoice();
    if (voice >= 0) {
        ASND_StopVoice(voice);
        ASND_SetVoice(voice, pcm_formats[effect], 44100, 0, (void*)pcm_buffers[effect], pcm_sizes[effect], 255, 255, NULL);
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
    for (int i = 0; i < MAX_SND_VOICES; ++i) {
        ASND_StopVoice(i);
    }
    ASND_Pause(1);
    ASND_End();
    for (int i = 0; i < 9; ++i) {
        if (pcm_buffers[i]) free(pcm_buffers[i]);
        pcm_buffers[i] = NULL;
    }
#else
    for (int i = 0; i < 9; ++i) {
        if (chunks[i]) Mix_FreeChunk(chunks[i]);
        chunks[i] = NULL;
    }
    Mix_CloseAudio();
#endif
    sound_ready = 0;
}
