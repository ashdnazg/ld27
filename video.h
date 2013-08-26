#ifndef __VIDEO_H__
#define __VIDEO_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include <SDL2/SDL.h>
#include "int_list.h"
#include "macros.h"
#include "assets.h"

#define RGBA 4
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000


typedef struct sprite_s {
    SDL_Texture *texture;
    SDL_Rect *rect;
} sprite_t;


typedef struct animation_s {
    unsigned int num_frames;
    sprite_t **frames;
} animation_t;

typedef struct renederable_s renderable_t;

typedef struct animation_playback_s {
    link_t animation_playbacks_link;
    bool loop;
    unsigned int interval;
    unsigned int total_steps;
    animation_t *animation;
    renderable_t *renderable;
} animation_playback_t;

struct renederable_s {
    link_t renderables_link;
    list_t *tweens;
    int x;
    int y;
    int depth;
    double scale;
    double angle;
    SDL_Point *center;
    SDL_RendererFlip  flip;
    sprite_t *sprite;
    sprite_t *default_sprite;
    animation_playback_t *animation_playback;
};

typedef struct render_manager_s {
    SDL_Renderer *renderer;
    list_t *renderables;
    list_t *animation_playbacks;
    asset_manager_t *textures;
    int x_offset;
    int y_offset;
} render_manager_t;


void exit_on_SDL_error(void * pt);
void exit_on_stbi_error(void * pt);
SDL_Texture* load_image(render_manager_t *r_manager, const char * path);
sprite_t * load_sprite(render_manager_t *r_manager, const char * path);
sprite_t ** load_sprite_sheet(render_manager_t *r_manager, const char * path, int spr_width, int spr_height, int padding, unsigned int *out_num_sprites);
void draw_image(render_manager_t *r_manager, SDL_Texture *texture, int x, int y);
sprite_t * sprite_new(SDL_Texture *texture, int x, int y, int w, int h);
void sprite_free(sprite_t *sprite);
void draw_sprite(render_manager_t *r_manager, sprite_t *sprite, int x, int y, double scale, double angle, SDL_Point *center, SDL_RendererFlip flip);

animation_t * animation_new(sprite_t **frames, unsigned int num_frames);
void animation_free(animation_t *animation);

render_manager_t * render_manager_new(SDL_Renderer *renderer);
void render_manager_free(render_manager_t *r_manager);
void render_manager_clear(render_manager_t *r_manager);

renderable_t * render_manager_create_renderable(render_manager_t *r_manager, sprite_t *default_sprite, int x, int y, int depth);
void renderable_free(renderable_t *renderable);

void render_manager_play_animation(render_manager_t *r_manager, renderable_t *renderable, animation_t *animation, unsigned int interval, bool loop);
void render_manager_stop_animation(render_manager_t *r_manager, renderable_t *renderable);
void render_manager_draw(render_manager_t *r_manager);
void render_manager_animate(render_manager_t *r_manager);


#ifdef __cplusplus
}
#endif

#endif
