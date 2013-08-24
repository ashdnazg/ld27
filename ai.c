#include "ai.h"
#include "game.h"
#include "actor.h"
#include "macros.h"
#include <assert.h>
#include <stdio.h>

void ai_security_cb(game_t *game, actor_t *actor) {
    int new_direction = 0;
    int i;
    if (game->player->x < actor->x) {
        new_direction |= BIT_W;
    } else if (game->player->x > actor->x) {
        new_direction |= BIT_E;
    }
    if (game->player->y < actor->y) {
        new_direction |= BIT_N;
    } else if (game->player->y > actor->y) {
        new_direction |= BIT_S;
    }
    list_for_each(game->actors, actor_t *, other){
        if (actor != other && other != game->player && (ABS(actor->x - other->x) + ABS(actor->y - other->y)) < CLOSE_THRESHOLD * 2) {
            if (actor->x > other->x) {
                if (actor->x - other->x == CLOSE_THRESHOLD) {
                    new_direction &= ~BIT_W;
                } else if (actor->x - other->x < CLOSE_THRESHOLD){
                    assert(actor->x - other->x >= 0);
                    new_direction &= ~BIT_W;
                    new_direction |= BIT_E;
                }
            } else {
                if (other->x - actor->x == CLOSE_THRESHOLD) {
                    new_direction &= ~BIT_E;
                } else if (other->x - actor->x < CLOSE_THRESHOLD){
                    assert(other->x - actor->x >= 0);
                    new_direction &= ~BIT_E;
                    new_direction |= BIT_W;
                }
            }
            if (actor->y > other->y) {
                if (actor->y - other->y == CLOSE_THRESHOLD) {
                    new_direction &= ~BIT_N;
                } else if (actor->y - other->y < CLOSE_THRESHOLD){
                    assert(actor->y - other->y >= 0);
                    new_direction &= ~BIT_N;
                    new_direction |= BIT_S;
                }
            } else {
                if (other->y - actor->y == CLOSE_THRESHOLD) {
                    new_direction &= ~BIT_S;
                } else if (other->y - actor->y < CLOSE_THRESHOLD){
                    assert(other->y - actor->y >= 0);
                    new_direction &= ~BIT_S;
                    new_direction |= BIT_N;
                }
            }
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