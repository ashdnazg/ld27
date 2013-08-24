#ifndef __SOUND_H__
#define __SOUND_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "int_list.h"
#include "macros.h"
#include <SDL2/SDL.h>

typedef struct sample_s {
    SDL_AudioSpec *spec;
    Uint8 *data;
    Uint32 len;
} sample_t;

typedef struct sample_playback_s {
    link_t played_samples_link;
    sample_t *sample;
    Uint32 pos;
    int volume;
    bool loop;
} sample_playback_t;

typedef struct sound_manager_s {
    list_t *played_samples;
    SDL_AudioSpec *spec;
    bool open;
} sound_manager_t;


sound_manager_t * sound_manager_new(void);
void sound_manager_free(sound_manager_t *s_manager);
sample_playback_t * sound_manager_play_sample(sound_manager_t *s_manager, sample_t *sample, int volume, bool loop);
sample_t * load_sample(sound_manager_t *s_manager, const char *path);
void sample_free(sample_t * sample);
void sample_playback_free(sample_playback_t * playback);

#ifdef __cplusplus
}
#endif

#endif
