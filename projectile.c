#include "projectile.h"
#include "mem_wrap.h"
#include "int_list.h"
#include "tween.h"
#include "macros.h"
#include <stdio.h>

projectile_t * projectile_new(game_t *game, sprite_t *sprite, int x, int y, int dest_x, int dest_y, int lifetime, bool trail, void **parent_ptr) {
    tween_value_t start, end;
    projectile_t *projectile = mem_alloc(sizeof(projectile_t));
    projectile->tweens = list_new(tween_t, tweens_link);
    projectile->sprite = sprite;
    projectile->x = x;
    projectile->y = y;
    projectile->old_x = x;
    projectile->old_y = y;
    projectile->lifetime = lifetime;
    projectile->trail = trail;
    projectile->renderables = mem_alloc(MAX_RENDERABLES * sizeof(renderable_t *));
    projectile->renderables[0] = NULL;
    projectile->num_renderables = 0;
    projectile->parent_ptr = parent_ptr;
    start.ival = x;
    end.ival = dest_x;
    tween_manager_add_tween(game->t_manager, projectile->tweens, &(projectile->x), TWEEN_TYPE_INT, lifetime , start, end, TWEEN_IN, linear_tween);
    
    start.ival = y;
    end.ival = dest_y;
    tween_manager_add_tween(game->t_manager, projectile->tweens, &(projectile->y), TWEEN_TYPE_INT, lifetime , start, end, TWEEN_IN, linear_tween);
    
    return projectile;
}

void draw_line(game_t *game, projectile_t *projectile) {
    bool first = TRUE;
    int sx, sy, err, e2;
    int x0 = projectile->old_x;
    int x1 = projectile->x;
    int y0 = projectile->old_y;
    int y1 = projectile->y;
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
    // printf("\nx0: %d,x1: %d,y0: %d,y1: %d",x0,x1,y0,y1);
    while (TRUE) {
        if (!first) {
            projectile->renderables[projectile->num_renderables] = render_manager_create_renderable(game->r_manager, projectile->sprite, x0 / 2, y0 / 3, PROJECTILE_DEPTH);
            ++(projectile->num_renderables);
        }
        first = FALSE;
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dy){
            err -= dy;
            x0 += sx; 
        }
        if (x0 == x1 && y0 == y1) {
            projectile->renderables[projectile->num_renderables] = render_manager_create_renderable(game->r_manager, projectile->sprite, x0 / 2, y0 / 3, PROJECTILE_DEPTH);
            ++(projectile->num_renderables);
            break;
        }
        if (e2 < dx){
            err += dx;
            y0 += sy; 
        }
    }
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
    if (projectile->lifetime != LIFETIME_PERMANENT) {
        if (projectile->lifetime == 0){
            projectile_free(projectile);
            return;
        }
        --(projectile->lifetime);
    }
    if(projectile->trail) {
        draw_line(game, projectile);
    } else {
        if (projectile->renderables[0] != NULL) {
            renderable_free(projectile->renderables[0]);
        }
        projectile->renderables[0] = render_manager_create_renderable(game->r_manager, projectile->sprite, projectile->x / 2, projectile->y / 3, PROJECTILE_DEPTH);
        projectile->num_renderables = 1;
    }
    projectile->old_x = projectile->x;
    projectile->old_y = projectile->y;
}

void projectile_free(projectile_t *projectile) {
    int i;
    if (projectile->parent_ptr != NULL) {
        *(projectile->parent_ptr) = NULL;
    }
    list_for_each(projectile->tweens, tween_t *, tween) {
        tween_free(tween);
    }
    list_free(projectile->tweens);
    for (i = 0;i < projectile->num_renderables;++i) {
        renderable_free(projectile->renderables[i]);
    }
    mem_free(projectile->renderables);
    mem_free(projectile);
}