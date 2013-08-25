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
#define MAX_RENDERABLES 100
#define PROJECTILE_DEPTH 3

typedef void (*hit_cb)(game_t *game, projectile_t *projectile, actor_t *actor);

struct projectile_s {
    list_t *tweens;
    sprite_t *sprite;
    int x;
    int y;
    int old_x;
    int old_y;
    int length;
    int lifetime;
    bool trail;
    renderable_t **renderables;
    unsigned int num_renderables;
    
    void **parent_ptr;
};

projectile_t * projectile_new(game_t *game, sprite_t *sprite, int x, int y, int dest_x, int dest_y, int lifetime, bool trail, void **parent_ptr);
void projectile_free(projectile_t *projectile);


void projectile_step(game_t *game, projectile_t *projectile);

#ifdef __cplusplus
}
#endif

#endif 
