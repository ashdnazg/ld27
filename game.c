#include "config.h"
#include "game.h"
#include "video.h"
#include "sound.h"
#include "tween.h"
#include "macros.h"
#include "mem_wrap.h"
#include "actor.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>

void print_achievements(game_t *game) {
    printf("\nbump_baddies: %d",       game->achievements.bump_baddies);
    printf("\ntackle_baddy: %d",       game->achievements.tackle_baddy);
    printf("\nwiggled: %d",            game->achievements.wiggled);
    printf("\nwiggled_at_player: %d",  game->achievements.wiggled_at_player);
    printf("\ninjured_red: %d",        game->achievements.injured_red);
    printf("\ninjured_blue: %d",       game->achievements.injured_blue);
    printf("\ninjured_police: %d",     game->achievements.injured_police);
    printf("\ntased_baddy: %d",        game->achievements.tased_baddy);
    printf("\ntased_police: %d",       game->achievements.tased_police);
    printf("\ntased_injured: %d",      game->achievements.tased_injured);
    printf("\nsurvived: %d",           game->achievements.survived);
}


game_t * game_new(render_manager_t *r_manager) {
    game_t *game = mem_alloc(sizeof(game_t));
    game->r_manager = r_manager;
    game->s_manager = sound_manager_new();
    game->t_manager = tween_manager_new();
    game->f_manager = font_manager_new();
    game->paused = TRUE;
    game->quit = FALSE;
    game->init = FALSE;
    game->running = TRUE;
    game->steps = 0;
    game->len_timer_caption = 0;
    game->timer_caption = NULL;
    game->logo = NULL;
    game->closing_delay = -1;
    game->sprites = asset_manager_new((free_cb_t) sprite_free);
    game->animations = asset_manager_new((free_cb_t) animation_free);
    game->samples = asset_manager_new((free_cb_t)sample_free);
    game->actors = list_new(actor_t, actors_link);
    memset(&game->achievements, 0, sizeof(achievements_t));
    memset(game->key_states, 0, sizeof(game->key_states));
    return game;
}

void game_free(game_t *game, bool restart) {
    if (game->timer_caption != NULL){
        mem_free(game->timer_caption);
    }
    if (game->logo != NULL){
        renderable_free(game->logo);
    }
    list_for_each(game->actors, actor_t *, actor) {
        actor_free(actor);
    }
    if (!restart) {
        sound_manager_free(game->s_manager);
        tween_manager_free(game->t_manager);
        font_manager_free(game->f_manager);
        asset_manager_free(game->sprites);
        asset_manager_free(game->animations);
        asset_manager_free(game->samples);
        list_free(game->actors);
        mem_free(game);
    } else {
        game->paused = TRUE;
        game->quit = FALSE;
        game->init = FALSE;
        game->running = TRUE;
        game->steps = 0;
        game->len_timer_caption = 0;
        game->timer_caption = NULL;
        game->logo = NULL;
        game->closing_delay = -1;
        memset(&game->achievements, 0, sizeof(achievements_t));
        memset(game->key_states, 0, sizeof(game->key_states));
    }
}

void game_init(game_t *game) {
    render_manager_create_renderable(game->r_manager, asset_manager_get(game->sprites, "field"), FIELD_X, FIELD_Y, FIELD_DEPTH);
    game->player = actor_new(game, ACTOR_TYPE_STREAKER, 1016, 1232, DIRECTION_N, NO_AI);
    actor_new(game, ACTOR_TYPE_SECURITY, 1205,1060, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_SECURITY, 874, 1360, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_SECURITY, 736,650, DIRECTION_S, ai_security_cb, NULL);
    actor_new(game, ACTOR_TYPE_SECURITY, 380,966, DIRECTION_S, ai_security_cb, NULL);
    
    actor_new(game, ACTOR_TYPE_POLICE, 1444,  728, DIRECTION_S, ai_police_cb, NULL);
    actor_new(game, ACTOR_TYPE_POLICE, 1095,  413, DIRECTION_S, ai_police_cb, NULL);
    actor_new(game, ACTOR_TYPE_POLICE, 92,   1286, DIRECTION_S, ai_police_cb, NULL);
    actor_new(game, ACTOR_TYPE_POLICE, 518,  1648, DIRECTION_S, ai_police_cb, NULL);
    
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,911, 1025,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,873, 953 ,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,779, 945 ,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,940, 794 ,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,1034,914 , DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,1192,972 ,DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,1209,861 , DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,1072,748 , DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,972, 594 ,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,836, 666 , DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_BLUE_PLAYER,1222,622 , DIRECTION_S, ai_player_cb, NULL);
    
    actor_new(game, ACTOR_TYPE_RED_PLAYER,794,1090,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,705,1037,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,508,1014,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,550,1160,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,643,1231, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,796,1284,DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,673,1449, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,539,1391, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,442,1226,  DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,344,1122, DIRECTION_S, ai_player_cb, NULL);
    actor_new(game, ACTOR_TYPE_RED_PLAYER,366,1408, DIRECTION_S, ai_player_cb, NULL);
    
    game->logo = render_manager_create_renderable(game->r_manager, asset_manager_get(game->sprites, "logo"), 0, 0, LOGO_DEPTH);
    sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "ambient"), 20, TRUE, NULL);
    game->init = TRUE;

    game->next_frame_time = SDL_GetTicks();
    game->last_time = SDL_GetTicks();
    game->time_to_next = 0;
    game->frames_skipped = 0;
    game->frames_this_second = 0;
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
    game->timer_caption = font_manager_print(game, game->f_manager, score_str, 10 - game->r_manager->x_offset, 10 - game->r_manager->y_offset, MAX_SCORE_LEN + 1, SCORE_DEPTH);

}

void game_step(game_t *game, bool draw) {
    renderable_t **broadcast_caption;
    
    if (game->closing_delay > 0) {
        --(game->closing_delay);
    } else if (game->closing_delay == 0){
        game->running = FALSE;
    } else if (!(game->player->active)){
        game->closing_delay = CLOSING_DELAY;
    }
    if (game->running && (!game->paused)){
        tween_manager_tween(game->t_manager);
        render_manager_animate(game->r_manager);
        if((game->steps % AI_FREQ) == 0) {
            ai_step(game);
        }
        collisions_step(game);
        list_for_each(game->actors, actor_t *, actor) {
            actor_step(game, actor);
        }
    }
    game->r_manager->x_offset = GAME_WIDTH / 2 - game->player->renderable->x + ACTOR_WIDTH / 2;
    game->r_manager->y_offset = GAME_HEIGHT / 2 - game->player->renderable->y + ACTOR_HEIGHT / 2;
    if (draw) {
        game->logo->x = -game->r_manager->x_offset + LOGO_X;
        game->logo->y = -game->r_manager->y_offset + LOGO_Y;
        update_timer(game);
        render_manager_draw(game->r_manager);
    }
    if(!(game->paused) && game->player->active){
        ++(game->steps);
        if (game->steps > SURVIVED){
            game->achievements.survived = TRUE;
            actor_set_state(game, game->player, STATE_STAND);
            game->player->active = FALSE;
            game->paused = TRUE;
            render_manager_create_renderable(game->r_manager, asset_manager_get(game->sprites, "black"), -game->r_manager->x_offset, -game->r_manager->y_offset, BLACK1_DEPTH);
            broadcast_caption = font_manager_print(game, game->f_manager, "Broadcast will return shortly", 30 - game->r_manager->x_offset, 100 - game->r_manager->y_offset,30, LOGO_DEPTH);
            mem_free(broadcast_caption);
        }
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
        if (direction != 0) {
            actor_set_dir(game, game->player, (actor_direction_t) direction);
        }
        actor_set_state(game, game->player, STATE_WIGGLE);
        return;
    }
    if (direction != 0) {
        actor_set_dir(game, game->player, (actor_direction_t) direction);
        actor_set_state(game, game->player, STATE_RUN);
    } else {
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
            game->paused = FALSE;
            update_player_state(game);
            break;
        // case SDL_SCANCODE_P:
            // printf("\nx: %d,y:%d", game->player->x, game->player->y);
            // break;
        default: break;
    }
}

void game_key_up(game_t *game, SDL_Scancode scan_code) {
    //assert(game->key_states[scan_code]);
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

bool inside_field(int x, int y) {
    return (((FIELD_N_X - FIELD_W_X)*(y - FIELD_W_Y) - (FIELD_N_Y - FIELD_W_Y)*(x - FIELD_W_X)) > 0) &&
           (((FIELD_W_X - FIELD_S_X)*(y - FIELD_S_Y) - (FIELD_W_Y - FIELD_S_Y)*(x - FIELD_S_X)) > 0) &&
           (((FIELD_N_X - FIELD_E_X)*(y - FIELD_E_Y) - (FIELD_N_Y - FIELD_E_Y)*(x - FIELD_E_X)) <= 0) &&
           (((FIELD_E_X - FIELD_S_X)*(y - FIELD_S_Y) - (FIELD_E_Y - FIELD_S_Y)*(x - FIELD_S_X)) <= 0);
}