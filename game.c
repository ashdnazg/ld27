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
    game->f_manager = font_manager_new();
    game->running = TRUE;
    game->steps = 0;
    game->len_timer_caption = 0;
    game->timer_caption = NULL;
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
    font_manager_free(game->f_manager);
    asset_manager_free(game->sprites);
    asset_manager_free(game->animations);
    asset_manager_free(game->samples);
    if (game->timer_caption != NULL){
        mem_free(game->timer_caption);
    }
    list_for_each(game->actors, actor_t *, actor) {
        actor_free(actor);
    }
    list_free(game->actors);
    mem_free(game);
}

void game_init(game_t *game) {
    init_field(game, 300, 0);
    game->player = actor_new(game, ACTOR_TYPE_STREAKER, 100, 500, DIRECTION_S, NO_AI);
    actor_new(game, ACTOR_TYPE_SECURITY, 600, 600, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_SECURITY, -200, 1200, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_SECURITY, 200, 400, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER, 200, 700, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER, -300, 1000, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER, -100, 800, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER, 400, 600, DIRECTION_S, ai_player_cb, NULL);
    sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "ambient"), 20, TRUE, NULL);
}

void ai_step(game_t *game) {
    list_for_each(game->actors, actor_t *, actor) {
        if (actor->ai_cb != NULL){
            actor->ai_cb(game, actor);
        }
    }
}
void collisions_step(game_t *game) {
    list_for_each(game->actors, actor_t *, actor) {
        list_for_each(game->actors, actor_t *, other) {
            if (actor != other){
                actor_collide(game, actor, other);
            }
        }
    }
}

void update_timer(game_t *game) {
    int i;
    int temp;
    int len;
    char score_str[MAX_SCORE_LEN + 2];
    char flipped_str[MAX_SCORE_LEN + 1];
    if (game->timer_caption != NULL) {
        for(i = 0;i < game->len_timer_caption;++i) {
            renderable_free(game->timer_caption[i]);
        }
        mem_free(game->timer_caption);
        game->timer_caption = NULL;
    }
    temp = (game->steps * STEP_INTERVAL) / 10;
    for (i = 0;i < MAX_SCORE_LEN;++i) {
        if (temp == 0){
            break;
        }
        flipped_str[i] = temp % 10 + 48;
        temp /= 10;
    }
    flipped_str[i] = 0;
    len = i;
    if (len > 2) {
        score_str[len + 1] = 0;
        score_str[len] = flipped_str[0];
        score_str[len - 1] = flipped_str[1];
        score_str[len - 2] = '.';
        for (i = 2;i < len;++i) {
            score_str[len - 1 - i] = flipped_str[i];
        }
        game->len_timer_caption = len + 1;
    } else if (len == 2) {
        score_str[4] = 0;
        score_str[3] = flipped_str[0];
        score_str[2] = flipped_str[1];
        score_str[1] = '.';
        score_str[0] = '0';
        game->len_timer_caption = 4;
    } else if (len == 1) {
        score_str[4] = 0;
        score_str[3] = flipped_str[0];
        score_str[2] = '0';
        score_str[1] = '.';
        score_str[0] = '0';
        game->len_timer_caption = 4;
    } else {
        score_str[4] = 0;
        score_str[3] = '0';
        score_str[2] = '0';
        score_str[1] = '.';
        score_str[0] = '0';
        game->len_timer_caption = 4;
    }
    game->timer_caption = font_manager_print(game, game->f_manager, score_str, 10 - game->r_manager->x_offset, 10 - game->r_manager->y_offset, MAX_SCORE_LEN + 1);

}

void game_step(game_t *game, bool draw) {
    render_manager_animate(game->r_manager);
    tween_manager_tween(game->t_manager);
    if((game->steps % AI_FREQ) == 0) {
        ai_step(game);
    }
    collisions_step(game);
    list_for_each(game->actors, actor_t *, actor) {
        actor_step(game, actor);
    }
    game->r_manager->x_offset = GAME_WIDTH / 2 - game->player->renderable->x + ACTOR_WIDTH / 2;
    game->r_manager->y_offset = GAME_HEIGHT / 2 - game->player->renderable->y + ACTOR_HEIGHT / 2;
    if (draw) {
        update_timer(game);
        render_manager_draw(game->r_manager);
    }
    if(game->player->active){
        ++(game->steps);
    }
}

void update_player_state(game_t *game) {
    int direction = 0;
    if (!(game->player->active)) {
        return;
    }
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
