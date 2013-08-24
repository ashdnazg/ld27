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
    if (ABS(actor->x - game->player->x) + ABS(actor->y - game->player->y) < JUMP_THRESHOLD) {
        actor_set_state(game, actor, STATE_JUMP);
    }
    new_direction = get_direction(actor->x, actor->y, game->player->x, game->player->y);
    list_for_each(game->actors, actor_t *, other){
        if (actor != other && other != game->player && other->active && (ABS(actor->x - other->x) + ABS(actor->y - other->y)) < CLOSE_THRESHOLD * 2) {
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