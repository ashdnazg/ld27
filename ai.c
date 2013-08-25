#include "ai.h"
#include "game.h"
#include "actor.h"
#include "macros.h"
#include <assert.h>
#include <stdio.h>

void ai_security_cb(game_t *game, actor_t *actor) {
    int new_direction = 0;
    int collision_direction;
    int i;
    if(!(actor->active)) {
        return;
    }
    if (DISTANCESQ(actor,game->player) < JUMP_THRESHOLD * JUMP_THRESHOLD) {
        actor_set_state(game, actor, STATE_JUMP);
        if (actor->voice == NULL) {
            actor->voice = sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "jump"), -1, FALSE, (void **) &(actor->voice));
        }
    }
    new_direction = get_direction(actor->x, actor->y, game->player->x, game->player->y);
    list_for_each(game->actors, actor_t *, other){
        if (actor != other && other != game->player && other->active && DISTANCESQ(actor,other) < CLOSE_THRESHOLD * 2) {
            collision_direction = get_direction(other->x, other->y, actor->x, actor->y);
            new_direction = collision_direction;
        }
    }
    if (new_direction == 0) {
        actor_set_state(game, actor, STATE_STAND);
    } else {
        if (new_direction != actor->direction) {
            actor_set_dir(game, actor, new_direction);
        }
        if (actor->state == STATE_STAND) {
            actor_set_state(game, actor, STATE_RUN);
        }
    }
}

void ai_player_cb(game_t *game, actor_t *actor) {
    int new_direction = 0;
    int i;
    if(!(actor->active) || actor->state_duration > 0) {
        return;
    }
    if (DISTANCESQ(actor, game->player) < CLOSE_THRESHOLD * CLOSE_THRESHOLD) {
        new_direction = get_direction(game->player->x, game->player->y, actor->x, actor->y);
        new_direction = ((new_direction << 1) & DIRECTION_MASK) + (new_direction >> (DIRECTION_BITS - 1));
        if (actor->voice == NULL) {
            actor->voice = sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "getaway"), -1, FALSE, (void **) &(actor->voice));
        }
    } else if (game->player->state == STATE_WIGGLE && DISTANCESQ(actor, game->player) < HORROR_THRESHOLD * HORROR_THRESHOLD) {
        new_direction = get_direction(game->player->x, game->player->y, actor->x, actor->y);
        if (actor->voice == NULL) {
            actor->voice = sound_manager_play_sample(game->s_manager, asset_manager_get(game->samples, "getaway"), -1, FALSE, (void **) &(actor->voice));
        }
    } else{
        list_for_each(game->actors, actor_t *, other){
            if (actor != other && other != game->player && other->active && DISTANCESQ(actor,other) < CLOSE_THRESHOLD * CLOSE_THRESHOLD) {
                new_direction = get_direction(other->x, other->y, actor->x, actor->y);
                new_direction = ((new_direction << 1) & DIRECTION_MASK) + (new_direction >> (DIRECTION_BITS - 1));
            }
        }
    }
    if (new_direction == 0) {
        new_direction = get_direction(actor->x, actor->y, game->player->x, game->player->y);
        actor_set_dir(game, actor, new_direction);
        actor_set_state(game, actor, STATE_STAND);
    } else {
        if (new_direction != actor->direction) {
            actor_set_dir(game, actor, new_direction);
        }
        if (actor->state == STATE_STAND) {
            actor_set_state(game, actor, STATE_RUN);
            actor->state_duration = FLEE_DURATION;
        }
    }
}