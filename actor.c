#include "actor.h"
#include "sound.h"
#include "game.h"
#include "video.h"
#include "string.h"
#include "macros.h"
#include "mem_wrap.h"
#include <stdio.h>

char * actor_get_graphics_name(actor_t *actor, actor_state_t state) {
    char *result = mem_alloc(256);
    char *type_str;
    char *result_ptr = result;
    char *state_str;
    char *dir_str;
    int len;
    int i = 0;
    
    switch(actor->type) {
        case ACTOR_TYPE_STREAKER:
            type_str = STREAKER_STR;
            break;
        case ACTOR_TYPE_SECURITY:
            type_str = SECURITY_STR;
            break;
        case ACTOR_TYPE_POLICE:
            type_str = POLICE_STR;
            break;
        case ACTOR_TYPE_PLAYERS:
            type_str = PLAYERS_STR;
            break;
        case ACTOR_TYPE_JUDGE:
            type_str = JUDGE_STR;
            break;
    }
    
    len = strlen(type_str);
    memcpy(result_ptr, type_str, len);
    result_ptr += len;
    *result_ptr = STR_SEP;
    ++result_ptr;
    
    switch(state) {
        case STATE_STAND:
            state_str = STAND_STR;
            break;
        case STATE_RUN:
            state_str = RUN_STR;
            break;
        case STATE_WIGGLE:
            state_str = WIGGLE_STR;
            break;
        case STATE_JUMP:
            state_str = JUMP_STR;
            break;
    }
    len = strlen(state_str);
    memcpy(result_ptr, state_str, len);
    result_ptr += len;
    *result_ptr = STR_SEP;
    ++result_ptr;
    
    
    switch(actor->direction) {
        case DIRECTION_N:
        case DIRECTION_NE:
        case DIRECTION_E:
        case DIRECTION_SE:
        case DIRECTION_S:
        case DIRECTION_SW:
        case DIRECTION_W:
        case DIRECTION_NW:
            dir_str = "s";
            break;
        default:
            printf("\ndirection error: %d", actor->direction);
            exit(1);
    }
    
    len = strlen(dir_str);
    memcpy(result_ptr, dir_str, len);
    result_ptr += len;
    *result_ptr = 0;
    return result;
}

void actor_set_state(game_t *game, actor_t *actor, actor_state_t state) {
    char *graphics_name;
    graphics_name = actor_get_graphics_name(actor, STATE_STAND);
    actor->renderable->sprite = asset_manager_get(game->sprites, graphics_name);
    mem_free(graphics_name);
    actor->renderable->default_sprite = actor->renderable->sprite;
    if (actor->state == STATE_WIGGLE && actor->voice != NULL && state != STATE_WIGGLE) {
        sample_playback_free(actor->voice);
        actor->voice = NULL;
    }
    
    switch(state) {
        case STATE_STAND:
            render_manager_stop_animation(game->r_manager, actor->renderable);
            break;
        case STATE_RUN:
            graphics_name = actor_get_graphics_name(actor, STATE_RUN);
            render_manager_play_animation(game->r_manager,  actor->renderable, asset_manager_get(game->animations, 
                                                    graphics_name), RUN_INTERVAL, TRUE);
            mem_free(graphics_name);
            break;
        case STATE_WIGGLE:
                graphics_name = actor_get_graphics_name(actor, STATE_WIGGLE);
                render_manager_play_animation(game->r_manager,  actor->renderable, asset_manager_get(game->animations, 
                                                        graphics_name), WIGGLE_INTERVAL, TRUE);
            mem_free(graphics_name);
            if (actor->voice == NULL) {
                actor->voice = sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "wiggle"), -1, TRUE);
            }
            break;
        case STATE_JUMP:
                graphics_name = actor_get_graphics_name(actor, STATE_JUMP);
                render_manager_play_animation(game->r_manager,  actor->renderable, asset_manager_get(game->animations, 
                                                        graphics_name), JUMP_INTERVAL, FALSE);
                actor->renderable->default_sprite = actor->renderable->animation_playback->animation->frames[actor->renderable->animation_playback->animation->num_frames - 1];
                actor->active = FALSE;
                actor->state_duration = JUMP_DURATION;
            mem_free(graphics_name);
            break;
    }
    actor->state = state;
}
void actor_set_pos(actor_t *actor, int x, int y) {
    actor->x = x;
    actor->y = y;
    actor->renderable->x = x / 2 - ACTOR_WIDTH / 2;
    actor->renderable->y = y / 3 - ACTOR_HEIGHT / 2;
}
void actor_set_dir(game_t *game, actor_t *actor, actor_direction_t direction) {
    actor->direction = direction;
    actor_set_state(game, actor, actor->state);
}
actor_t * actor_new(game_t *game, actor_type_t type, int x, int y, actor_direction_t direction, ai_cb_t ai_cb, void *ai_params) {
    actor_t *actor = mem_alloc(sizeof(actor_t));
    actor->renderable = render_manager_create_renderable(game->r_manager, NULL, 0, 0, ACTOR_DEPTH);
    actor->state = STATE_STAND;
    actor->type = type;
    actor->voice = NULL;
    actor->active = TRUE;
    actor->state_duration = 0;
    actor->ai_cb = ai_cb;
    actor->ai_params = ai_params;
    link_init(&(actor->actors_link));
    list_insert_tail(game->actors, actor);
    actor_set_pos(actor, x, y);
    actor_set_dir(game, actor, direction);
    return actor;
}

void actor_free(actor_t *actor) {
    link_remove_from_list(&(actor->actors_link));
    renderable_free(actor->renderable);
    mem_free(actor);
}

actor_direction_t get_direction(int x1, int y1, int x2, int y2) {
    int ratio;
    int direction = 0;
    if (x1 == x2) {
        return (y1 > y2 ? DIRECTION_N : (y1 < y2 ? DIRECTION_S : 0));
    }
    if (y1 == y2) {
        return (x1 > x2 ? DIRECTION_W : DIRECTION_E);
    }
    
    ratio = 2 * ABS(x1 - x2) / ABS(y1 - y2);
    if (x2 < x1) {
        direction |= BIT_W;
    } else {
        direction |= BIT_E;
    }
    if (y2 < y1) {
        direction |= BIT_N;
    } else {
        direction |= BIT_S;
    }
    if (ratio >= 4) {
        direction &= (~BIT_N & ~BIT_S);
    } else if (ratio == 0) {
        direction &= (~BIT_E & ~BIT_W);
    }
    return direction;
}

void actor_move(game_t *game, actor_t *actor) {
    actor_set_pos(actor, actor->x + ((actor->direction & BIT_E) ? 1 : ((actor->direction & BIT_W) ? -1 : 0)) * (actor->direction & (BIT_N | BIT_S) ? 1 : 2) ,
                  actor->y + ((actor->direction & BIT_S) ? 1 : ((actor->direction & BIT_N) ? -1 : 0)) * (actor->direction & (BIT_W | BIT_E) ? 1 : 2));
}

void actor_step(game_t *game, actor_t *actor) {

    switch(actor->state) {
        case STATE_STAND: break;
        case STATE_RUN:
            actor_move(game, actor);
            break;
        case STATE_JUMP:
            if (actor->state_duration > 0) { 
                actor_move(game, actor);
                //actor_move(game, actor);
                --(actor->state_duration);
            }
            break;
        case STATE_WIGGLE: break;
    }
}