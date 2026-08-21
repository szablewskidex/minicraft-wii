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

void sound_init(void);
void sound_play(SoundEffect effect);
void sound_cleanup(void);

#endif // SOUND_H
