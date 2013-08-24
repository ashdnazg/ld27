#ifndef __ACTOR_H__
#define __ACTOR_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct actor_s actor_t;

#include "game.h"
#include "video.h"
#include "sound.h"
#include "ai.h"
#define ACTOR_WIDTH 5
#define ACTOR_HEIGHT 6

#define RUN_INTERVAL 5
#define WIGGLE_INTERVAL 5


typedef enum actor_state_e {
    STATE_STAND,
    STATE_RUN,
    STATE_WIGGLE,
} actor_state_t;

#define ACTOR_DEPTH 0
#define NO_AI NULL, NULL


#define STR_SEP '_'
#define STAND_STR "stand"
#define RUN_STR "run"
#define WIGGLE_STR "wiggle"

typedef enum actor_type_e {
    ACTOR_TYPE_STREAKER,
    ACTOR_TYPE_SECURITY,
    ACTOR_TYPE_POLICE,
    ACTOR_TYPE_PLAYERS,
    ACTOR_TYPE_JUDGE,
} actor_type_t;

#define STREAKER_STR "streaker"
#define SECURITY_STR "security"
#define POLICE_STR "police"
#define PLAYERS_STR "players"
#define JUDGE_STR "judge"


#define BIT_N (1 << 0)
#define BIT_E (1 << 1)
#define BIT_S (1 << 2)
#define BIT_W (1 << 3)

typedef void (*ai_cb_t)(game_t *game, actor_t *actor);


typedef enum actor_direction_e {
    DIRECTION_N = 1,
    DIRECTION_NE = 3,
    DIRECTION_E = 2,
    DIRECTION_SE = 6,
    DIRECTION_S = 4,
    DIRECTION_SW = 12,
    DIRECTION_W = 8,
    DIRECTION_NW = 9,
} actor_direction_t;

struct actor_s {
    link_t actors_link;
    actor_type_t type;
    int x;
    int y;
    actor_state_t state;
    actor_direction_t direction;
    renderable_t *renderable;
    sample_playback_t *voice;
    ai_cb_t ai_cb;
    void *ai_params;
};

actor_t * actor_new(game_t *game, actor_type_t type, int x, int y, actor_direction_t direction, ai_cb_t ai_cb, void *ai_params);

void actor_free(actor_t *actor);

void actor_set_state(game_t *game, actor_t *actor, actor_state_t state);
void actor_set_pos(actor_t *actor, int x, int y);
void actor_set_dir(game_t *game, actor_t *actor, actor_direction_t direction);

void actor_step(game_t *game, actor_t *actor);

#ifdef __cplusplus
}
#endif

#endif
