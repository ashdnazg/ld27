#include "projectile.h"
#include "mem_wrap.h"
#include "int_list.h"
#include "macros.h"
#include <stdio.h>

projectile_t * projectile_new(game_t *game, sprite_t *sprite, int x, int y, int dest_x, int dest_y, unsigned int speed, unsigned int length, int lifetime, bool trail, void **parent_ptr) {
    projectile_t *projectile = mem_alloc(sizeof(projectile_t));
    projectile->renderables = mem_alloc(MAX_RENDERABLES * sizeof(renderable_t *));
    projectile->sprite = sprite;
    projectile->start_x = x;
    projectile->start_y = y;
    projectile->x = x;
    projectile->y = y;
    projectile->dest_x = dest_x;
    projectile->dest_y = dest_y;
    projectile->speed = speed;
    projectile->length = length;
    projectile->lifetime = lifetime;
    projectile->trail = trail;
    projectile->renderables[0] = NULL;
    projectile->num_renderables = 0;
    projectile->parent_ptr = parent_ptr;
    
    return projectile;
}
void put_renderable(game_t *game, projectile_t *projectile, int x, int y) {
    projectile->renderables[projectile->num_renderables] = render_manager_create_renderable(game->r_manager, projectile->sprite, x / 2, y / 3, PROJECTILE_DEPTH);
    ++(projectile->num_renderables);
    projectile->length -= 1;
}
void check_collisions(game_t *game, projectile_t *projectile) {
    
}
int draw_line(game_t *game, projectile_t *projectile, bool trail, int len) {
    bool first = TRUE;
    unsigned int remaining = len;
    int sx, sy, err, e2;
    int x0 = projectile->x;
    int y0 = projectile->y;
    int x1 = projectile->dest_x;
    int y1 = projectile->dest_y;
    int dx = ABS(x1 - x0);
    int dy = ABS(y1 - y0);
    if (x0 < x1) {
        sx = 1;
    } else {
        sx = -1;
    }
    if (y0 < y1) {
        sy = 1;
    } else {
        sy = -1;
    }
    err = dx - dy;
    //printf("\nx0: %d,x1: %d,y0: %d,y1: %d",x0,x1,y0,y1);
    while (TRUE) {
        if (!first) {
            if (trail) {
                put_renderable(game, projectile, x0, y0);
            }
            --remaining;
        }
        first = FALSE;
        if (x0 == x1 && y0 == y1) {
            projectile->dest_x += projectile->dest_x - projectile->start_x;
            projectile->dest_y += projectile->dest_y - projectile->start_y;
            break;
        }
        if (remaining == 0) {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dy){
            err -= dy;
            x0 += sx; 
        }
        if (x0 == x1 && y0 == y1) {
            if (trail) {
                put_renderable(game, projectile, x0, y0);
            }
            --remaining;
            projectile->dest_x += projectile->dest_x - projectile->start_x;
            projectile->dest_y += projectile->dest_y - projectile->start_y;
            break;
        }
        if (remaining == 0) {
            break;
        }
        if (e2 < dx){
            err += dx;
            y0 += sy; 
        }
    }
    projectile->x = x0;
    projectile->y = y0;
    return remaining;
   // loop
     // plot(x0,y0)
     // if x0 = x1 and y0 = y1 exit loop
     // e2 := 2*err
     // if e2 > -dy then 
       // err := err - dy
       // x0 := x0 + sx
     // end if
     // if x0 = x1 and y0 = y1 then 
       // plot(x0,y0)
       // exit loop
     // end if
     // if e2 <  dx then 
       // err := err + dx
       // y0 := y0 + sy 
     // end if
   // end loop
}

void projectile_step(game_t *game, projectile_t *projectile){
    int remaining = MIN(projectile->speed, projectile->length);
    int next_remaining;
    if (projectile->lifetime != LIFETIME_PERMANENT) {
        if (projectile->lifetime == 0){
            projectile_free(projectile);
            return;
        }
        --(projectile->lifetime);
    }
    while (remaining > 0) {
        remaining = draw_line(game, projectile, projectile->trail, MIN(remaining, projectile->length));
    }
    if(!(projectile->trail)) {
        if (projectile->renderables[0] != NULL) {
            renderable_free(projectile->renderables[0]);
            projectile->num_renderables = 0;
        }
        put_renderable(game, projectile, projectile->x, projectile->y);
    }
}

void projectile_free(projectile_t *projectile) {
    int i;
    if (projectile->parent_ptr != NULL) {
        *(projectile->parent_ptr) = NULL;
    }
    for (i = 0;i < projectile->num_renderables;++i) {
        renderable_free(projectile->renderables[i]);
    }
    mem_free(projectile->renderables);
    mem_free(projectile);
}