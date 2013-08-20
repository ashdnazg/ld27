#include "game.h"
#include "video.h"
#include "sound.h"
#include "tween.h"
#include "macros.h"
#include "mem_wrap.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>

game_t * game_new(render_manager_t *r_manager) {
    game_t *game = mem_alloc(sizeof(game_t));
    game->r_manager = r_manager;
    game->s_manager = sound_manager_new();
    game->t_manager = tween_manager_new();
    game->running = TRUE;
    game->sprites = asset_manager_new((free_cb_t) sprite_free);
    game->animations = asset_manager_new((free_cb_t) animation_free);
    game->samples = asset_manager_new((free_cb_t)sample_free);
    memset(game->key_states, 0, sizeof(game->key_states));
    return game;
}

void game_free(game_t *game) {
    sound_manager_free(game->s_manager);
    tween_manager_free(game->t_manager);
    asset_manager_free(game->sprites);
    asset_manager_free(game->animations);
    asset_manager_free(game->samples);
    
    mem_free(game);
}

void game_step(game_t *game, bool draw) {
    render_manager_animate(game->r_manager);
    tween_manager_tween(game->t_manager);
    if (draw) {
        render_manager_draw(game->r_manager);
    }
}


void game_key_down(game_t *game, SDL_Scancode scan_code) {
    if (game->key_states[scan_code]) {
        return;
    } else {
        game->key_states[scan_code] = TRUE;
    }
    switch(scan_code) {
        default: break;
    }
}

void game_key_up(game_t *game, SDL_Scancode scan_code) {
    assert(game->key_states[scan_code]);
    game->key_states[scan_code] = FALSE;
    switch(scan_code) {
        default: break;
    }
}
