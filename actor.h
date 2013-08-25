#ifndef __ACTOR_H__
#define __ACTOR_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct actor_s actor_t;
#include "projectile.h"
#include "game.h"
#include "video.h"
#include "sound.h"
#include "ai.h"
#define ACTOR_WIDTH 5
#define ACTOR_HEIGHT 6

#define COLLISION_RADIUS 7

#define DISTANCESQ(object_x,object_y) (((object_x)->x - (object_y->x)) * ((object_x)->x - (object_y->x)) + ((object_x)->y - (object_y->y)) * ((object_x)->y - (object_y->y)))

#define RUN_INTERVAL 5
#define WIGGLE_INTERVAL 5
#define JUMP_INTERVAL 5
#define AIM_INTERVAL 10
#define SHOCK_INTERVAL 5

#define JUMP_DURATION 20
#define AIM_DURATION 30
#define SHOCK_DURATION 60

#define TASE_SOUND_MOMENT 20
#define TASE_SHOT_MOMENT 10

typedef enum actor_state_e {
    STATE_STAND,
    STATE_RUN,
    STATE_WIGGLE,
    STATE_JUMP,
    STATE_AIM,
    STATE_SHOCK,
} actor_state_t;

#define ACTOR_DEPTH 0
#define NO_AI NULL, NULL


#define STR_SEP '_'
#define STAND_STR "stand"
#define RUN_STR "run"
#define WIGGLE_STR "wiggle"
#define JUMP_STR "jump"
#define AIM_STR "aim"

typedef enum actor_type_e {
    ACTOR_TYPE_STREAKER,
    ACTOR_TYPE_SECURITY,
    ACTOR_TYPE_POLICE,
    ACTOR_TYPE_BLUE_PLAYER,
    ACTOR_TYPE_RED_PLAYER,
    ACTOR_TYPE_JUDGE,
} actor_type_t;

#define STREAKER_STR "streaker"
#define SECURITY_STR "security"
#define POLICE_STR "police"
#define BLUE_PLAYER_STR "bplayer"
#define RED_PLAYER_STR "rplayer"
#define JUDGE_STR "judge"


#define BIT_N (1 << 0)
#define BIT_E (1 << 1)
#define BIT_S (1 << 2)
#define BIT_W (1 << 3)

#define DIRECTION_MASK 15
#define DIRECTION_BITS 4


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
    int state_duration;
    actor_direction_t direction;
    renderable_t *renderable;
    sample_playback_t *voice;
    projectile_t *projectile;
    bool active;
    int aim_x;
    int aim_y;
    ai_cb_t ai_cb;
    void *ai_params;
};

actor_t * actor_new(game_t *game, actor_type_t type, int x, int y, actor_direction_t direction, ai_cb_t ai_cb, void *ai_params);

void actor_free(actor_t *actor);

void actor_set_state(game_t *game, actor_t *actor, actor_state_t state);
void actor_set_pos(actor_t *actor, int x, int y);
void actor_set_dir(game_t *game, actor_t *actor, actor_direction_t direction);

void actor_step(game_t *game, actor_t *actor);
void actor_collide(game_t *game, actor_t *actor, actor_t *other);
actor_direction_t get_direction(int x1, int y1, int x2, int y2);

#ifdef __cplusplus
}
#endif

#endif
