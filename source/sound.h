#ifndef SOUND_H
#define SOUND_H

typedef enum {
    SND_SELECT,
    SND_CONFIRM,
    SND_CRAFT,
    SND_PICKUP,
    SND_PLAYERHURT,
    SND_MONSTERHURT,
    SND_DEATH,
    SND_BOSSDEATH,
    SND_EXPLODE
} SoundEffect;

extern int g_sfxVolume;
extern int g_uiVolume;
extern int g_musicVolume;

void sound_init(void);
void sound_tick(void);
void sound_play(SoundEffect effect);
void sound_cleanup(void);

void music_play_menu(void);
void music_play_game(void);
void music_set_level(int level_index);
void music_stop(void);
void music_tick(void);

#endif // SOUND_H
