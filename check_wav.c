#include <stdio.h>
#include "source/sound_data.h"

void check_wav(const char* name, const unsigned char* data, int size) {
    if (size < 44) return;
    int channels = data[22] | (data[23] << 8);
    int sample_rate = data[24] | (data[25] << 8) | (data[26] << 16) | (data[27] << 24);
    int bits_per_sample = data[34] | (data[35] << 8);
    printf("%s: %d channels, %d Hz, %d bits\n", name, channels, sample_rate, bits_per_sample);
}

int main() {
    check_wav("select", sound_select, sound_select_size);
    check_wav("confirm", sound_confirm, sound_confirm_size);
    check_wav("craft", sound_craft, sound_craft_size);
    check_wav("pickup", sound_pickup, sound_pickup_size);
    check_wav("playerhurt", sound_playerhurt, sound_playerhurt_size);
    check_wav("monsterhurt", sound_monsterhurt, sound_monsterhurt_size);
    check_wav("death", sound_death, sound_death_size);
    check_wav("bossdeath", sound_bossdeath, sound_bossdeath_size);
    check_wav("explode", sound_explode, sound_explode_size);
    return 0;
}
