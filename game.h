#ifndef __GAME_H__
#define __GAME_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct game_s game_t;

#include "actor.h"
#include "video.h"
#include "assets.h"
#include "sound.h"
#include "macros.h"
#include "tween.h"
#include "font.h"
#include <SDL2/SDL.h>

#define MAX_SCORE_LEN 10

struct game_s {
    render_manager_t *r_manager;
    sound_manager_t *s_manager;
    tween_manager_t *t_manager;
    font_manager_t *f_manager;
    asset_manager_t *sprites;
    asset_manager_t *animations;
    asset_manager_t *samples;
    list_t *actors;
    actor_t *player;
    renderable_t **timer_caption;
    unsigned int len_timer_caption;
    bool running;
    bool key_states[SDL_NUM_SCANCODES] ;
    int steps;
};


game_t * game_new(render_manager_t *r_manager);
void game_free(game_t *game);

void game_step(game_t *game, bool draw);
void game_init(game_t *game);


void game_key_down(game_t *game, SDL_Scancode scan_code);
void game_key_up(game_t *game, SDL_Scancode scan_code);
#ifdef __cplusplus
}
#endif

#endif
