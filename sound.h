#ifndef __SOUND_H__
#define __SOUND_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "int_list.h"
#include "macros.h"
#include <SDL2/SDL.h>

typedef struct sample_s sample_t;

typedef struct sample_playback_s sample_playback_t;

typedef struct sound_manager_s sound_manager_t;


sound_manager_t * sound_manager_new(void);
void sound_manager_free(sound_manager_t *s_manager);
void sound_manager_clear(sound_manager_t *s_manager);
sample_playback_t * sound_manager_play_sample(sound_manager_t *s_manager, sample_t *sample, int volume, bool loop, void **parent_ptr);
sample_t * load_sample(sound_manager_t *s_manager, const char *path);
void sample_free(sample_t * sample);
void sample_playback_free(sample_playback_t * playback);

#ifdef __cplusplus
}
#endif

#endif
