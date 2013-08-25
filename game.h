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
#define FIELD_X 0
#define FIELD_Y 0
#define FIELD_DEPTH -1000

#define FIELD_W_X 40
#define FIELD_W_Y 1270
#define FIELD_N_X 1070
#define FIELD_N_Y 350
#define FIELD_E_X 1540
#define FIELD_E_Y 770
#define FIELD_S_X 510
#define FIELD_S_Y 1690

#define LOGO_X 260
#define LOGO_Y 210

typedef struct achievements_s {
    bool bump_baddies;
    bool tackle_baddy;
    bool wiggled;
    bool injured_red;
    bool injured_blue;
    bool injured_police;
    bool tased_baddy;
    bool tased_police;
    bool survived;
} achievements_t;

struct game_s {
    render_manager_t *r_manager;
    sound_manager_t *s_manager;
    tween_manager_t *t_manager;
    font_manager_t *f_manager;
    asset_manager_t *sprites;
    asset_manager_t *animations;
    asset_manager_t *samples;
    achievements_t *achievements;
    list_t *actors;
    actor_t *player;
    renderable_t **timer_caption;
    renderable_t *logo;
    unsigned int len_timer_caption;
    bool init;
    bool paused;
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

bool inside_field(int x, int y);
#ifdef __cplusplus
}
#endif

#endif
