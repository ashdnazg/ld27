#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef struct projectile_s projectile_t;
#include "int_list.h"
#include "game.h"
#include "video.h"
#include "macros.h"
#include "actor.h"

#define LIFETIME_PERMANENT -1
#define MAX_RENDERABLES 256
#define PROJECTILE_DEPTH 3

#define PROJECTILE_RADIUS 5

typedef void (*hit_cb_t)(game_t *game, projectile_t *projectile, actor_t *actor);

struct projectile_s {
    sprite_t *sprite;
    int start_x;
    int start_y;
    int x;
    int y;
    int dest_x;
    int dest_y;
    unsigned int length;
    unsigned int speed;
    int lifetime;
    bool trail;
    renderable_t **renderables;
    unsigned int num_renderables;
    hit_cb_t hit_cb;
    actor_t *parent;
};

projectile_t * projectile_new(game_t *game, sprite_t *sprite, int x, int y, int dest_x, int dest_y, unsigned int speed, unsigned int length, int lifetime, bool trail, hit_cb_t hit_cb, actor_t *parent);
void projectile_free(projectile_t *projectile);

void projectile_step(game_t *game, projectile_t *projectile);

void taser_hit(game_t *game, projectile_t *projectile, actor_t *actor);

#ifdef __cplusplus
}
#endif

#endif 
