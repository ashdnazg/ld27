#include "config.h"
#include "game.h"
#include "video.h"
#include "sound.h"
#include "tween.h"
#include "macros.h"
#include "mem_wrap.h"
#include "field.h"
#include "actor.h"
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
    game->actors = list_new(actor_t, actors_link);
    memset(game->key_states, 0, sizeof(game->key_states));
    return game;
}

void game_free(game_t *game) {
    sound_manager_free(game->s_manager);
    tween_manager_free(game->t_manager);
    asset_manager_free(game->sprites);
    asset_manager_free(game->animations);
    asset_manager_free(game->samples);
    
    list_for_each(game->actors, actor_t *, actor) {
        actor_free(actor);
    }
    list_free(game->actors);
    mem_free(game);
}

void game_init(game_t *game) {
    init_field(game, 300, 0);
    game->player = actor_new(game, ACTOR_TYPE_STREAKER, 100, 500, DIRECTION_S, NO_AI);
    actor_new(game, ACTOR_TYPE_STREAKER, 600, 600, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_STREAKER, -300, 1200, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_STREAKER, 200, 400, DIRECTION_S, ai_security_cb, NULL);
    sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "ambient"), 20, TRUE);
}

void ai_step(game_t *game) {
    list_for_each(game->actors, actor_t *, actor) {
        if (actor->ai_cb != NULL){
            actor->ai_cb(game, actor);
        }
    }
}

void game_step(game_t *game, bool draw) {
    render_manager_animate(game->r_manager);
    tween_manager_tween(game->t_manager);
    ai_step(game);
    list_for_each(game->actors, actor_t *, actor) {
        actor_step(game, actor);
    }
    game->r_manager->x_offset = GAME_WIDTH / 2 - game->player->renderable->x + ACTOR_WIDTH / 2;
    game->r_manager->y_offset = GAME_HEIGHT / 2 - game->player->renderable->y + ACTOR_HEIGHT / 2;
    if (draw) {
        render_manager_draw(game->r_manager);
    }
}

void update_player_state(game_t *game) {
    int direction = 0;
    // Make sure they're not pressing up and down (or left and right) at the same time.
    if ((!(game->key_states[SDL_SCANCODE_UP]) || !(game->key_states[SDL_SCANCODE_DOWN])) &&
        (!(game->key_states[SDL_SCANCODE_LEFT]) || !(game->key_states[SDL_SCANCODE_RIGHT]))) {
        if ((game->key_states[SDL_SCANCODE_UP])) {
            direction |= BIT_N;
        }
        if ((game->key_states[SDL_SCANCODE_DOWN])) {
            direction |= BIT_S;
        }
        if ((game->key_states[SDL_SCANCODE_RIGHT])) {
            direction |= BIT_E;
        }
        if ((game->key_states[SDL_SCANCODE_LEFT])) {
            direction |= BIT_W;
        }
    }
    
    if (game->key_states[SDL_SCANCODE_SPACE]) {
        printf("\nwiggle");
        if (direction != 0) {
            actor_set_dir(game, game->player, (actor_direction_t) direction);
        }
        actor_set_state(game, game->player, STATE_WIGGLE);
        return;
    }
    if (direction != 0) {
        printf("\nrun");
        actor_set_dir(game, game->player, (actor_direction_t) direction);
        actor_set_state(game, game->player, STATE_RUN);
    } else {
        printf("\nstand");
        actor_set_state(game, game->player, STATE_STAND);
    }
}

void game_key_down(game_t *game, SDL_Scancode scan_code) {
    if (game->key_states[scan_code]) {
        return;
    } else {
        game->key_states[scan_code] = TRUE;
    }
    switch(scan_code) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_SPACE:
            update_player_state(game);
            break;
        default: break;
    }
}

void game_key_up(game_t *game, SDL_Scancode scan_code) {
    assert(game->key_states[scan_code]);
    game->key_states[scan_code] = FALSE;
    switch(scan_code) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_SPACE:
            update_player_state(game);
            break;
        default: break;
    }
}
