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

extern const uint8_t sound_music_mp3[];
extern const size_t sound_music_mp3_size;
extern const uint8_t sound_cave_ogg[];
extern const size_t sound_cave_ogg_size;

static Mix_Chunk* chunks[9] = {0};
static const uint8_t* sound_ptrs[9] = {
    sound_select, sound_confirm, sound_craft, sound_pickup,
    sound_playerhurt, sound_monsterhurt, sound_death, sound_bossdeath, sound_explode
};
static const size_t sound_sizes[9] = {
    sound_select_size, sound_confirm_size, sound_craft_size, sound_pickup_size,
    sound_playerhurt_size, sound_monsterhurt_size, sound_death_size, sound_bossdeath_size, sound_explode_size
};

int g_sfxVolume = 10;   // 0..10
int g_uiVolume = 10;    // 0..10
int g_musicVolume = 8;  // 0..10

static int sound_ready = 0;
static Mix_Music* g_music_surface = NULL;
static Mix_Music* g_music_cave = NULL;
static int g_currentLevelIndex = 3; // 3 = Surface, <3 = Caves
static int music_mode = 0; // 0 = stopped, 1 = menu (loop), 2 = game (ambient)
static int music_ambient_timer = 0;

void sound_init(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0) {
        Mix_AllocateChannels(16);
        for (int i = 0; i < 9; ++i) {
            chunks[i] = Mix_LoadWAV_RW(SDL_RWFromConstMem(sound_ptrs[i], (int)sound_sizes[i]), 1);
        }
        sound_ready = 1;
        printf("SDL_mixer initialized successfully with 16 channels!\n");

        // Load surface background music
        if (sound_music_mp3_size > 0) {
            SDL_RWops* rw = SDL_RWFromConstMem(sound_music_mp3, (int)sound_music_mp3_size);
            if (rw) g_music_surface = Mix_LoadMUS_RW(rw);
        }
        if (!g_music_surface) {
            g_music_surface = Mix_LoadMUS("sd:/apps/minicraft/music.mp3");
            if (!g_music_surface) g_music_surface = Mix_LoadMUS("music.mp3");
            if (!g_music_surface) g_music_surface = Mix_LoadMUS("assets/sound/music1.mp3");
        }

        // Load cave background ambiance
        if (sound_cave_ogg_size > 0) {
            SDL_RWops* rw = SDL_RWFromConstMem(sound_cave_ogg, (int)sound_cave_ogg_size);
            if (rw) g_music_cave = Mix_LoadMUS_RW(rw);
        }
        if (!g_music_cave) {
            g_music_cave = Mix_LoadMUS("sd:/apps/minicraft/cave.ogg");
            if (!g_music_cave) g_music_cave = Mix_LoadMUS("cave.ogg");
            if (!g_music_cave) g_music_cave = Mix_LoadMUS("assets/sound/cave.ogg");
        }

        if (g_music_surface) printf("Surface music loaded successfully!\n");
        if (g_music_cave) printf("Cave ambiance loaded successfully!\n");
    } else {
        printf("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
    }
}

static int sound_last_played[9] = {0};
static int sound_frame_counter = 0;

void sound_tick(void) {
    sound_frame_counter++;
    music_tick();
}

void sound_play(SoundEffect effect) {
    if (!sound_ready) return;
    if (effect < 0 || effect >= 9) return;

    // Prevent identical sound spamming within 3 frames
    if (sound_frame_counter - sound_last_played[effect] < 3) {
        return;
    }
    sound_last_played[effect] = sound_frame_counter;

    int vol = 128;
    if (effect == SND_SELECT || effect == SND_CONFIRM) {
        if (g_uiVolume <= 0) return;
        vol = (g_uiVolume * 128) / 10;
    } else {
        if (g_sfxVolume <= 0) return;
        vol = (g_sfxVolume * 128) / 10;
    }

    if (chunks[effect]) {
        Mix_VolumeChunk(chunks[effect], vol);
        Mix_PlayChannel(-1, chunks[effect], 0);
    }
}

void music_play_menu(void) {
    if (!sound_ready) return;
    if (music_mode == 1 && Mix_PlayingMusic()) return;

    music_mode = 1;
    if (g_musicVolume > 0 && g_music_surface) {
        Mix_VolumeMusic((g_musicVolume * 128) / 10);
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(g_music_surface, -1);
        }
    }
}

void music_play_game(void) {
    if (!sound_ready) return;
    if (music_mode == 2) return;

    music_mode = 2;
    // If music is already playing, let it finish naturally without restarting
    if (!Mix_PlayingMusic()) {
        music_ambient_timer = 120 + (rand() % 240);
    }
}

void music_set_level(int level_index) {
    if (g_currentLevelIndex == level_index) return;
    int was_cave = (g_currentLevelIndex < 3);
    int is_cave = (level_index < 3);
    g_currentLevelIndex = level_index;

    if (music_mode == 2) {
        if (was_cave != is_cave) {
            // Environment changed (Surface <-> Cave): stop current track immediately!
            if (Mix_PlayingMusic()) {
                Mix_HaltMusic();
            }
            // Set short initial ambiance trigger (1.5 to 3 seconds)
            music_ambient_timer = 90 + (rand() % 120);
        }
    }
}

void music_stop(void) {
    music_mode = 0;
    if (sound_ready && Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

void music_tick(void) {
    if (!sound_ready) return;

    if (g_musicVolume <= 0) {
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        return;
    }

    Mix_VolumeMusic((g_musicVolume * 128) / 10);

    if (music_mode == 1) {
        // Menu music (Continuous looping)
        if (g_music_surface && !Mix_PlayingMusic()) {
            Mix_PlayMusic(g_music_surface, -1);
        }
    } else if (music_mode == 2) {
        // In-game ambient music
        Mix_Music* active_track = (g_currentLevelIndex < 3) ? g_music_cave : g_music_surface;
        if (!active_track) return;

        if (Mix_PlayingMusic()) {
            // Currently playing, let it play through without looping
            return;
        }

        if (music_ambient_timer > 0) {
            --music_ambient_timer;
        } else {
            // Start playing track once
            Mix_PlayMusic(active_track, 1);
            // Random interval before next ambiance:
            // Cave: 40 to 100 seconds (2400 to 6000 frames)
            // Surface: 60 to 180 seconds (3600 to 10800 frames)
            if (g_currentLevelIndex < 3) {
                music_ambient_timer = 2400 + (rand() % 3600);
            } else {
                music_ambient_timer = 3600 + (rand() % 5400);
            }
        }
    }
}

void sound_cleanup(void) {
    if (!sound_ready) return;
    music_stop();
    if (g_music_surface) {
        Mix_FreeMusic(g_music_surface);
        g_music_surface = NULL;
    }
    if (g_music_cave) {
        Mix_FreeMusic(g_music_cave);
        g_music_cave = NULL;
    }
    for (int i = 0; i < 9; ++i) {
        if (chunks[i]) Mix_FreeChunk(chunks[i]);
        chunks[i] = NULL;
    }
    Mix_CloseAudio();
    sound_ready = 0;
}
