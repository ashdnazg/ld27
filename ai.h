#ifndef __AI_H__
#define __AI_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "actor.h"
#include "game.h"

#define CLOSE_THRESHOLD 40
#define JUMP_THRESHOLD 24
#define TASE_THRESHOLD 150
#define HORROR_THRESHOLD 200

#define FLEE_DURATION 15

void ai_security_cb(game_t *game, actor_t *actor);
void ai_police_cb(game_t *game, actor_t *actor);
void ai_player_cb(game_t *game, actor_t *actor);

#ifdef __cplusplus
}
#endif

#endif
