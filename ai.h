#ifndef __AI_H__
#define __AI_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "actor.h"
#include "game.h"

#define CLOSE_THRESHOLD 10

void ai_security_cb(game_t *game, actor_t *actor);

#ifdef __cplusplus
}
#endif

#endif
